#include <Geode/Geode.hpp>
#include <Geode/utils/general.hpp>

using namespace geode::prelude;

#include <Geode/modify/ColorSelectPopup.hpp>
class $modify(MyMenuLayer, ColorSelectPopup) {
	bool init(EffectGameObject * p0, CCArray * p1, ColorAction * p2) {
		if (!ColorSelectPopup::init(p0, p1, p2)) {
			return false;
		}

		auto pasteHexButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_pasteBtn_001.png"),
			this,
			menu_selector(MyMenuLayer::onPasteButton)
		);

		auto clearHexButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png"),
			this,
			menu_selector(MyMenuLayer::onClearButton)
		);

		if (auto mainLayer = static_cast<CCLayer*>(this->getChildren()->objectAtIndex(0)))
		{
			if (auto hexTextInputNode = mainLayer->getChildByTag(11))
			{
				hexTextInputNode->setID("hex-input-text-field");
			}

			if (auto colorMenu = FindFirstChildOfType<CCMenu>(mainLayer))
			{
				colorMenu->addChild(pasteHexButton);
				colorMenu->addChild(clearHexButton);

				pasteHexButton->setID("paste-hex-button"_spr);
				clearHexButton->setID("clear-hex-button"_spr);

				pasteHexButton->m_baseScale = 0.75f;
				clearHexButton->m_baseScale = 0.75f;
				pasteHexButton->setScale(0.75f, 0.75f);
				clearHexButton->setScale(0.75f, 0.75f);

				pasteHexButton->setPosition(-104, 134);
				clearHexButton->setPosition(-203, 134);

				colorMenu->updateLayout();
			}
		}
		return true;
	}

	enum HexActionsEnum
	{
		Paste,
		Clear
	};

	void onHexAction(CCObject* sender, HexActionsEnum hexAction)
	{
		if (auto button = typeinfo_cast<CCMenuItemSpriteExtra*>(sender)) {
			// button's parent is CCMenu, grandparent is CCLayer. 
			if (auto hexTextInputNode = static_cast<CCTextInputNode*>(button->getParent()->getParent()->getChildByID("hex-input-text-field")))
			{
				if (hexAction == HexActionsEnum::Paste)
				{
					std::string clipboardText = geode::utils::clipboard::read();
					if (clipboardText.length() > 6)
					{
						clipboardText = clipboardText.substr(0, 6);
					}
					hexTextInputNode->setString(clipboardText);
				}
				else if (hexAction == HexActionsEnum::Clear)
				{
					hexTextInputNode->setString("");
				}
			}
		}
	}

	void onPasteButton(CCObject*sender) {
		onHexAction(sender, HexActionsEnum::Paste);
	}

	void onClearButton(CCObject* sender) {
		onHexAction(sender, HexActionsEnum::Clear);
	}

	template <typename T>
	T* FindFirstChildOfType(CCNode* parent) {
		if (!parent) return nullptr;

		auto children = parent->getChildren();
		for (int i = 0; i < children->count(); ++i) {
			auto child = children->objectAtIndex(i);
			if (auto casted = typeinfo_cast<T*>(child)) {
				return casted;
			}
		}
		return nullptr;
	}
};