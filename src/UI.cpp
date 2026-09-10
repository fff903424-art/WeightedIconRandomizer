#include "../include/UI.hpp"
#include "../include/Config.hpp"
#include "../include/Randomizer.hpp"

#include <Geode/Geode.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/binding/CCScrollLayerExt.hpp>

#include <algorithm>
#include <cmath>

using namespace geode::prelude;

namespace wir {

static ButtonSprite* makeButton(char const* text, float scale = .55f) {
    auto* sprite = ButtonSprite::create(text, "goldFont.fnt", "GJ_button_01.png", .8f);
    sprite->setScale(scale);
    return sprite;
}

class EditorPopup final : public Popup<> {
    std::vector<CubeEntry> m_entries;
    CCNode* m_content = nullptr;

    bool init() {
        if (!Popup<>::init(480.f, 360.f)) return false;
        setTitle("Weighted Cube Entries");
        m_entries = loadEntries();
        rebuild();
        return true;
    }

    void rebuild() {
        if (m_content) m_content->removeFromParent();
        m_content = CCNode::create();
        m_mainLayer->addChild(m_content);

        auto* info = CCLabelBMFont::create("Source  |  Cube/name  |  Weight  |  On  |  Remove", "goldFont.fnt");
        info->setScale(.32f);
        info->setPosition({240.f, 145.f});
        m_content->addChild(info);

        float y = 116.f;
        constexpr float rowHeight = 42.f;

        for (size_t i = 0; i < m_entries.size(); ++i) {
            if (y < -130.f) break;
            auto const& e = m_entries[i];
            auto* row = CCNode::create();
            row->setPosition({0.f, y});
            m_content->addChild(row);

            auto* source = CCMenuItemSpriteExtra::create(
                makeButton(e.source == IconSource::Vanilla ? "Vanilla" : "More", .42f),
                this,
                menu_selector(EditorPopup::onSource)
            );
            source->setTag(static_cast<int>(i));
            source->setPosition({70.f, 0.f});
            row->addChild(source);

            auto* input = TextInput::create(175.f, e.source == IconSource::Vanilla ? "Cube ID" : "More Icons name", "bigFont.fnt");
            input->setScale(.44f);
            input->setPosition({200.f, 0.f});
            input->setTag(static_cast<int>(i));
            input->setString(e.source == IconSource::Vanilla ? fmt::format("{}", e.vanillaID) : e.moreIconsName, false);
            input->setCallback([this, i](std::string const& text) {
                if (i >= m_entries.size()) return;
                if (m_entries[i].source == IconSource::Vanilla) {
                    try { m_entries[i].vanillaID = std::max(1, std::stoi(text)); }
                    catch (...) {}
                } else {
                    m_entries[i].moreIconsName = text;
                }
                saveEntries(m_entries);
            });
            row->addChild(input);

            auto* weight = TextInput::create(70.f, "Weight", "bigFont.fnt");
            weight->setScale(.44f);
            weight->setPosition({305.f, 0.f});
            weight->setCommonFilter(CommonFilter::Float);
            weight->setString(fmt::format("{}", e.weight), false);
            weight->setCallback([this, i](std::string const& text) {
                if (i >= m_entries.size()) return;
                try {
                    double value = std::stod(text);
                    if (std::isfinite(value) && value >= 0.0) m_entries[i].weight = value;
                } catch (...) {}
                saveEntries(m_entries);
            });
            row->addChild(weight);

            auto* toggle = CCMenuItemToggler::createWithStandardSprites(
                this, menu_selector(EditorPopup::onToggle), .55f
            );
            toggle->setTag(static_cast<int>(i));
            toggle->toggle(!e.enabled);
            toggle->setPosition({365.f, 0.f});
            row->addChild(toggle);

            auto* remove = CCMenuItemSpriteExtra::create(
                makeButton("X", .42f), this, menu_selector(EditorPopup::onRemove)
            );
            remove->setTag(static_cast<int>(i));
            remove->setPosition({415.f, 0.f});
            row->addChild(remove);

            y -= rowHeight;
        }

        auto* add = CCMenuItemSpriteExtra::create(
            makeButton("Add", .5f), this, menu_selector(EditorPopup::onAdd)
        );
        add->setPosition({110.f, -157.f});
        m_content->addChild(add);

        auto* reset = CCMenuItemSpriteExtra::create(
            makeButton("Reset", .5f), this, menu_selector(EditorPopup::onReset)
        );
        reset->setPosition({240.f, -157.f});
        m_content->addChild(reset);

        auto* close = CCMenuItemSpriteExtra::create(
            makeButton("Close", .5f), this, menu_selector(EditorPopup::onClosePopup)
        );
        close->setPosition({370.f, -157.f});
        m_content->addChild(close);
    }

    void onAdd(CCObject*) {
        m_entries.push_back(CubeEntry{});
        saveEntries(m_entries);
        rebuild();
    }

    void onRemove(CCObject* sender) {
        auto index = static_cast<size_t>(sender->getTag());
        if (index >= m_entries.size()) return;
        m_entries.erase(m_entries.begin() + static_cast<ptrdiff_t>(index));
        saveEntries(m_entries);
        rebuild();
    }

    void onSource(CCObject* sender) {
        auto index = static_cast<size_t>(sender->getTag());
        if (index >= m_entries.size()) return;
        auto& entry = m_entries[index];
        entry.source = entry.source == IconSource::Vanilla ? IconSource::MoreIcons : IconSource::Vanilla;
        saveEntries(m_entries);
        rebuild();
    }

    void onToggle(CCObject* sender) {
        auto index = static_cast<size_t>(sender->getTag());
        if (index >= m_entries.size()) return;
        m_entries[index].enabled = !m_entries[index].enabled;
        saveEntries(m_entries);
    }

    void onReset(CCObject*) {
        resetEntries();
        m_entries = loadEntries();
        rebuild();
    }

    void onClosePopup(CCObject*) {
        this->removeFromParentAndCleanup(true);
    }

public:
    static EditorPopup* create() {
        auto* ret = new EditorPopup();
        if (ret->init()) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

void openEditor() {
    if (auto* popup = EditorPopup::create()) popup->show();
}

void testRandomize() {
    randomizeCube();
}

void resetFromUI() {
    resetEntries();
    FLAlertLayer::create("Weighted Icon Randomizer", "Configuration reset to the default example.", "OK")->show();
}

class ButtonSettingV3 final : public SettingV3 {
public:
    std::string action;

    static Result<std::shared_ptr<SettingV3>> parse(
        std::string const& key,
        std::string const& modID,
        matjson::Value const& json
    ) {
        auto ret = std::make_shared<ButtonSettingV3>();
        auto root = checkJson(json, "Weighted Icon Randomizer button setting");
        GEODE_UNWRAP(ret->parseBaseProperties(key, modID, json));
        root.has("action").into(ret->action);
        root.checkUnknownKeys();
        return root.ok(std::static_pointer_cast<SettingV3>(ret));
    }

    SettingNodeV3* createNode(float width) override;
    bool load(matjson::Value const&) override { return true; }
    bool save(matjson::Value&) const override { return true; }
    bool isDefaultValue() const override { return true; }
    void reset() override {}
};

class ButtonSettingNodeV3 final : public SettingNodeV3 {
    ButtonSprite* m_sprite = nullptr;
    CCMenuItemSpriteExtra* m_button = nullptr;

    bool init(std::shared_ptr<ButtonSettingV3> setting, float width) {
        if (!SettingNodeV3::init(setting, width)) return false;

        char const* text = "Open";
        if (setting->action == "test") text = "Randomize";
        else if (setting->action == "reset") text = "Reset";
        else if (setting->action == "editor") text = "Configure";

        m_sprite = makeButton(text, .55f);
        m_button = CCMenuItemSpriteExtra::create(m_sprite, this, menu_selector(ButtonSettingNodeV3::onClick));
        getButtonMenu()->addChildAtPosition(m_button, Anchor::Center);
        getButtonMenu()->setContentWidth(105.f);
        getButtonMenu()->updateLayout();
        updateState(nullptr);
        return true;
    }

    void onClick(CCObject*) {
        auto setting = getSetting();
        if (setting->action == "editor") openEditor();
        else if (setting->action == "test") testRandomize();
        else if (setting->action == "reset") resetFromUI();
    }

public:
    static ButtonSettingNodeV3* create(std::shared_ptr<ButtonSettingV3> setting, float width) {
        auto* ret = new ButtonSettingNodeV3();
        if (ret->init(std::move(setting), width)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    void onCommit() override {}
    void onResetToDefault() override {}
    bool hasUncommittedChanges() const override { return false; }
    bool hasNonDefaultValue() const override { return false; }

    std::shared_ptr<ButtonSettingV3> getSetting() const {
        return std::static_pointer_cast<ButtonSettingV3>(SettingNodeV3::getSetting());
    }
};

SettingNodeV3* ButtonSettingV3::createNode(float width) {
    return ButtonSettingNodeV3::create(
        std::static_pointer_cast<ButtonSettingV3>(shared_from_this()), width
    );
}

void registerSettings() {
    auto result = Mod::get()->registerCustomSettingType("wir-button", &ButtonSettingV3::parse);
    if (result.isErr()) {
        log::error("Failed to register custom setting type: {}", result.unwrapErr());
    }
}

} // namespace wir
