#include <UI.hpp>
#include <Config.hpp>
#include <Randomizer.hpp>

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/CCTextInputNode.hpp>

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace geode::prelude;

namespace wir {

class EditorPopup final : public Popup {
protected:
    std::vector<CubeEntry> m_entries;
    int m_selected = 0;

    CCMenu* m_menu = nullptr;

    CCLabelBMFont* m_entryLabel = nullptr;
    CCLabelBMFont* m_sourceLabel = nullptr;
    CCLabelBMFont* m_enabledLabel = nullptr;
    CCLabelBMFont* m_statusLabel = nullptr;

    CCTextInputNode* m_idInput = nullptr;
    CCTextInputNode* m_weightInput = nullptr;
    CCTextInputNode* m_nameInput = nullptr;

    bool init() {
        if (!Popup::init(520.f, 400.f))
            return false;

        setTitle("Weighted Cube Randomizer");

        m_entries = loadEntries();

        if (m_entries.empty())
            m_entries = defaultEntries();

        m_menu = CCMenu::create();

        if (!m_menu)
            return false;

        m_menu->setPosition({0.f, 0.f});
        m_mainLayer->addChild(m_menu);

        buildUI();
        refreshUI();

        return true;
    }

    void buildUI() {
        auto size = m_mainLayer->getContentSize();

        auto title = CCLabelBMFont::create(
            "Cube Entry Editor",
            "bigFont.fnt"
        );

        if (title) {
            title->setScale(.55f);
            title->setPosition({
                size.width / 2.f,
                size.height - 48.f
            });
            m_mainLayer->addChild(title);
        }

        m_entryLabel = CCLabelBMFont::create(
            "",
            "bigFont.fnt"
        );

        if (m_entryLabel) {
            m_entryLabel->setScale(.42f);
            m_entryLabel->setPosition({
                size.width / 2.f,
                size.height - 85.f
            });
            m_mainLayer->addChild(m_entryLabel);
        }

        m_sourceLabel = CCLabelBMFont::create(
            "",
            "bigFont.fnt"
        );

        if (m_sourceLabel) {
            m_sourceLabel->setScale(.38f);
            m_sourceLabel->setAnchorPoint({0.f, .5f});
            m_sourceLabel->setPosition({
                45.f,
                size.height - 125.f
            });
            m_mainLayer->addChild(m_sourceLabel);
        }

        m_enabledLabel = CCLabelBMFont::create(
            "",
            "bigFont.fnt"
        );

        if (m_enabledLabel) {
            m_enabledLabel->setScale(.38f);
            m_enabledLabel->setAnchorPoint({0.f, .5f});
            m_enabledLabel->setPosition({
                45.f,
                size.height - 155.f
            });
            m_mainLayer->addChild(m_enabledLabel);
        }

        auto idText = CCLabelBMFont::create(
            "Vanilla Cube ID:",
            "bigFont.fnt"
        );

        if (idText) {
            idText->setScale(.34f);
            idText->setAnchorPoint({0.f, .5f});
            idText->setPosition({
                45.f,
                size.height - 195.f
            });
            m_mainLayer->addChild(idText);
        }

        m_idInput = CCTextInputNode::create(
            145.f,
            32.f,
            "Cube ID",
            "bigFont.fnt"
        );

        if (m_idInput) {
            m_idInput->setScale(.65f);
            m_idInput->setPosition({
                145.f,
                size.height - 195.f
            });
            m_mainLayer->addChild(m_idInput);
        }

        auto weightText = CCLabelBMFont::create(
            "Weight:",
            "bigFont.fnt"
        );

        if (weightText) {
            weightText->setScale(.34f);
            weightText->setAnchorPoint({0.f, .5f});
            weightText->setPosition({
                45.f,
                size.height - 235.f
            });
            m_mainLayer->addChild(weightText);
        }

        m_weightInput = CCTextInputNode::create(
            145.f,
            32.f,
            "Weight",
            "bigFont.fnt"
        );

        if (m_weightInput) {
            m_weightInput->setScale(.65f);
            m_weightInput->setPosition({
                145.f,
                size.height - 235.f
            });
            m_mainLayer->addChild(m_weightInput);
        }

        auto nameText = CCLabelBMFont::create(
            "More Icons name:",
            "bigFont.fnt"
        );

        if (nameText) {
            nameText->setScale(.34f);
            nameText->setAnchorPoint({0.f, .5f});
            nameText->setPosition({
                45.f,
                size.height - 275.f
            });
            m_mainLayer->addChild(nameText);
        }

        m_nameInput = CCTextInputNode::create(
            240.f,
            32.f,
            "Custom icon name",
            "bigFont.fnt"
        );

        if (m_nameInput) {
            m_nameInput->setScale(.65f);
            m_nameInput->setPosition({
                185.f,
                size.height - 275.f
            });
            m_mainLayer->addChild(m_nameInput);
        }

        m_statusLabel = CCLabelBMFont::create(
            "",
            "bigFont.fnt"
        );

        if (m_statusLabel) {
            m_statusLabel->setScale(.30f);
            m_statusLabel->setPosition({
                size.width / 2.f,
                55.f
            });
            m_mainLayer->addChild(m_statusLabel);
        }

        createButton(
            "Prev",
            {70.f, 105.f},
            menu_selector(EditorPopup::onPrevious)
        );

        createButton(
            "Next",
            {145.f, 105.f},
            menu_selector(EditorPopup::onNext)
        );

        createButton(
            "Add",
            {220.f, 105.f},
            menu_selector(EditorPopup::onAdd)
        );

        createButton(
            "Remove",
            {300.f, 105.f},
            menu_selector(EditorPopup::onRemove)
        );

        createButton(
            "Source",
            {385.f, 105.f},
            menu_selector(EditorPopup::onSource)
        );

        createButton(
            "Enabled",
            {460.f, 105.f},
            menu_selector(EditorPopup::onEnabled)
        );

        createButton(
            "Save",
            {120.f, 25.f},
            menu_selector(EditorPopup::onSave)
        );

        createButton(
            "Test",
            {205.f, 25.f},
            menu_selector(EditorPopup::onTest)
        );

        createButton(
            "Reset",
            {290.f, 25.f},
            menu_selector(EditorPopup::onReset)
        );
    }

    void createButton(
        char const* text,
        CCPoint position,
        SEL_MenuHandler callback
    ) {
        auto sprite = ButtonSprite::create(text);

        if (!sprite)
            return;

        sprite->setScale(.55f);

        auto button = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            callback
        );

        if (!button)
            return;

        button->setPosition(position);
        m_menu->addChild(button);
    }

    void refreshUI() {
        if (m_entries.empty()) {
            m_selected = 0;

            if (m_entryLabel)
                m_entryLabel->setString("No entries");

            if (m_sourceLabel)
                m_sourceLabel->setString("Add an entry.");

            if (m_enabledLabel)
                m_enabledLabel->setString("");

            if (m_idInput)
                m_idInput->setString("");

            if (m_weightInput)
                m_weightInput->setString("");

            if (m_nameInput)
                m_nameInput->setString("");

            updateStatus();
            return;
        }

        if (m_selected < 0)
            m_selected = 0;

        if (m_selected >= static_cast<int>(m_entries.size()))
            m_selected = static_cast<int>(m_entries.size()) - 1;

        auto const& entry = m_entries[m_selected];

        if (m_entryLabel) {
            auto entryText = fmt::format(
                "Entry {}/{}",
                m_selected + 1,
                m_entries.size()
            );

            m_entryLabel->setString(entryText.c_str());
        }

        if (m_sourceLabel) {
            char const* source =
                entry.source == IconSource::Vanilla
                    ? "Source: Vanilla"
                    : "Source: More Icons";

            m_sourceLabel->setString(source);
        }

        if (m_enabledLabel) {
            auto enabledText = fmt::format(
                "Enabled: {}",
                entry.enabled ? "YES" : "NO"
            );

            m_enabledLabel->setString(enabledText.c_str());
        }

        if (m_idInput) {
            m_idInput->setString(
                std::to_string(
                    std::max(1, entry.vanillaID)
                )
            );
        }

        if (m_weightInput) {
            std::ostringstream weight;

            weight << std::fixed
                   << std::setprecision(3)
                   << entry.weight;

            m_weightInput->setString(weight.str());
        }

        if (m_nameInput)
            m_nameInput->setString(entry.moreIconsName);

        updateStatus();
    }

    void updateStatus() {
        if (!m_statusLabel)
            return;

        if (m_entries.empty()) {
            m_statusLabel->setString("No entries");
            return;
        }

        auto const& entry = m_entries[m_selected];

        std::string text;

        if (entry.source == IconSource::Vanilla) {
            text = fmt::format(
                "Vanilla Cube {} | Weight {:.3f}",
                entry.vanillaID,
                entry.weight
            );
        }
        else {
            text = fmt::format(
                "More Icons '{}' | Weight {:.3f}",
                entry.moreIconsName,
                entry.weight
            );
        }

        m_statusLabel->setString(text.c_str());
    }

    void commitCurrent() {
        if (m_entries.empty())
            return;

        if (m_selected < 0 ||
            m_selected >= static_cast<int>(m_entries.size()))
            return;

        auto& entry = m_entries[m_selected];

        if (m_idInput) {
            try {
                auto id = std::stoi(m_idInput->getString());

                if (id > 0)
                    entry.vanillaID = id;
            }
            catch (...) {
            }
        }

        if (m_weightInput) {
            try {
                auto weight =
                    std::stod(m_weightInput->getString());

                if (std::isfinite(weight) && weight >= 0.0)
                    entry.weight = weight;
            }
            catch (...) {
            }
        }

        if (m_nameInput)
            entry.moreIconsName = m_nameInput->getString();

        saveEntries(m_entries);
    }

    void onPrevious(CCObject*) {
        commitCurrent();

        if (m_entries.empty())
            return;

        --m_selected;

        if (m_selected < 0)
            m_selected =
                static_cast<int>(m_entries.size()) - 1;

        refreshUI();
    }

    void onNext(CCObject*) {
        commitCurrent();

        if (m_entries.empty())
            return;

        ++m_selected;

        if (m_selected >= static_cast<int>(m_entries.size()))
            m_selected = 0;

        refreshUI();
    }

    void onAdd(CCObject*) {
        commitCurrent();

        CubeEntry entry;
        entry.source = IconSource::Vanilla;
        entry.vanillaID = 1;
        entry.moreIconsName = "";
        entry.weight = 1.0;
        entry.enabled = true;

        m_entries.push_back(entry);

        m_selected =
            static_cast<int>(m_entries.size()) - 1;

        saveEntries(m_entries);
        refreshUI();
    }

    void onRemove(CCObject*) {
        if (m_entries.empty())
            return;

        if (m_selected < 0 ||
            m_selected >= static_cast<int>(m_entries.size()))
            return;

        m_entries.erase(
            m_entries.begin() + m_selected
        );

        if (m_selected >= static_cast<int>(m_entries.size()))
            m_selected =
                static_cast<int>(m_entries.size()) - 1;

        if (m_selected < 0)
            m_selected = 0;

        saveEntries(m_entries);
        refreshUI();
    }

    void onSource(CCObject*) {
        if (m_entries.empty())
            return;

        commitCurrent();

        auto& entry = m_entries[m_selected];

        if (entry.source == IconSource::Vanilla) {
            entry.source = IconSource::MoreIcons;

            if (entry.moreIconsName.empty())
                entry.moreIconsName = "my_custom_icon";
        }
        else {
            entry.source = IconSource::Vanilla;
        }

        saveEntries(m_entries);
        refreshUI();
    }

    void onEnabled(CCObject*) {
        if (m_entries.empty())
            return;

        commitCurrent();

        m_entries[m_selected].enabled =
            !m_entries[m_selected].enabled;

        saveEntries(m_entries);
        refreshUI();
    }

    void onSave(CCObject*) {
        commitCurrent();
        updateStatus();

        if (m_statusLabel)
            m_statusLabel->setString("Saved!");
    }

    void onTest(CCObject*) {
        commitCurrent();

        randomizeCube();

        if (m_statusLabel)
            m_statusLabel->setString(
                "Randomized cube!"
            );
    }

    void onReset(CCObject*) {
        m_entries = defaultEntries();
        m_selected = 0;

        saveEntries(m_entries);
        refreshUI();

        if (m_statusLabel)
            m_statusLabel->setString(
                "Entries reset."
            );
    }

public:
    static EditorPopup* create() {
        auto ret = new EditorPopup();

        if (ret->init()) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }
};

void openEditor() {
    auto popup = EditorPopup::create();

    if (!popup)
        return;

    popup->show();
}

void testRandomize() {
    randomizeCube();
}

void resetFromUI() {
    resetEntries();

    FLAlertLayer::create(
        "Weighted Icon Randomizer",
        "Configuration reset to the default example.",
        "OK"
    )->show();
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

        auto root = checkJson(
            json,
            "Weighted Icon Randomizer button setting"
        );

        GEODE_UNWRAP(
            ret->parseBaseProperties(
                key,
                modID,
                json
            )
        );

        root.has("action").into(ret->action);
        root.checkUnknownKeys();

        return root.ok(
            std::static_pointer_cast<SettingV3>(ret)
        );
    }

    SettingNodeV3* createNode(float width) override;

    bool load(matjson::Value const&) override {
        return true;
    }

    bool save(matjson::Value&) const override {
        return true;
    }

    bool isDefaultValue() const override {
        return true;
    }

    void reset() override {}
};

class ButtonSettingNodeV3 final : public SettingNodeV3 {
    ButtonSprite* m_sprite = nullptr;
    CCMenuItemSpriteExtra* m_button = nullptr;

    bool init(
        std::shared_ptr<ButtonSettingV3> setting,
        float width
    ) {
        if (!SettingNodeV3::init(setting, width))
            return false;

        char const* text = "Open";

        if (setting->action == "test")
            text = "Randomize";
        else if (setting->action == "reset")
            text = "Reset";
        else if (setting->action == "editor")
            text = "Configure";

        m_sprite = ButtonSprite::create(text);

        if (!m_sprite)
            return false;

        m_sprite->setScale(.55f);

        m_button = CCMenuItemSpriteExtra::create(
            m_sprite,
            this,
            menu_selector(ButtonSettingNodeV3::onClick)
        );

        if (!m_button)
            return false;

        getButtonMenu()->addChildAtPosition(
            m_button,
            Anchor::Center
        );

        getButtonMenu()->setContentWidth(105.f);
        getButtonMenu()->updateLayout();

        updateState(nullptr);

        return true;
    }

    void onClick(CCObject*) {
        auto setting = getSetting();

        if (setting->action == "editor")
            openEditor();
        else if (setting->action == "test")
            testRandomize();
        else if (setting->action == "reset")
            resetFromUI();
    }

public:
    static ButtonSettingNodeV3* create(
        std::shared_ptr<ButtonSettingV3> setting,
        float width
    ) {
        auto ret = new ButtonSettingNodeV3();

        if (ret->init(std::move(setting), width)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }

    void onCommit() override {}

    void onResetToDefault() override {}

    bool hasUncommittedChanges() const override {
        return false;
    }

    bool hasNonDefaultValue() const override {
        return false;
    }

    std::shared_ptr<ButtonSettingV3> getSetting() const {
        return std::static_pointer_cast<ButtonSettingV3>(
            SettingNodeV3::getSetting()
        );
    }
};

SettingNodeV3* ButtonSettingV3::createNode(float width) {
    return ButtonSettingNodeV3::create(
        std::static_pointer_cast<ButtonSettingV3>(
            shared_from_this()
        ),
        width
    );
}

void registerSettings() {
    auto result =
        Mod::get()->registerCustomSettingType(
            "wir-button",
            &ButtonSettingV3::parse
        );

    if (result.isErr()) {
        log::error(
            "Failed to register custom setting type: {}",
            result.unwrapErr()
        );
    }
}

} // namespace wir
