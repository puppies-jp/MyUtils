# FireFox 拡張機能

ここではFireFoxの拡張機能について書いてみる

- `manifest.json`サンプル これをFireFoxブラウザが読み込むことで拡張機能として実行できる様になる。

```json
{

  "manifest_version": 2,
  "name": "Chat Rec",
  "version": "1.0",

  "description": "",
// URLにマッチすると、指定したJSが読み込まれる  
  "content_scripts": [
    {
      "matches": ["*://*/*"],
      "js": ["js/listUp.js"]
    }
  ],

// 何だっけ？
  "permissions": [
    "activeTab"
  ],

// ブラウザでボタンクリックから表示される処理
  "browser_action": {
    "default_icon":"img/icon.png",
    "default_title": "ChatRec",
    "default_popup": "menu.html"
  },
// サイドバーの設定
  "sidebar_action": {
    "default_icon":"img/icon.png",
    "default_title": "My sidebar",
    "default_panel": "menu.html"
  }
}
```
