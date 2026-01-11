# Unityについて

Unityについてまとめる

- [Linuxインストール](./index.md)

MVCモデルの置き換えにおいて、

- Model -> `ScriptableObject` 
- View -> `Monobehaviour`,`GameObject`
- Controller -> `△Monobehaviour`

## 🧠MonoBehaviour（モノビヘイビア）
MonoBehaviour（モノビヘイビア）とは、Unityのスクリプトの基本クラスであり、GameObjectに機能を追加するための土台です。UnityでC#スクリプトを書くとき、ほとんどの場合このMonoBehaviourを継承して使います。

MonoBehaviourの役割
- Unityのイベントライフサイクルメソッド（Awake, Start, Updateなど）を使えるようにする。
- GameObjectにスクリプトとしてアタッチできるようにする。
- Unityエンジンと連携して、時間経過やユーザー操作に応じた処理を実行できる。

🧩 MonoBehaviourを継承しないとどうなる？
- Unityのイベントメソッドは使えません。
- GameObjectにスクリプトをアタッチできません。
- Unityエディタ上でインスペクターに表示されません。


## 📦ScriptableObject
📦 ScriptableObjectの使いどころ
- アイテムやスキルのデータ定義
- ゲーム設定（難易度、音量など）
- ステータスや属性の共有
- 複数オブジェクト間で使い回すデータ

🧩 例：ScriptableObjectでアイテムデータを定義

```cs
[CreateAssetMenu(fileName = "NewItem", menuName = "Game/Item")]
public class ItemData : ScriptableObject
{
    public string itemName;
    public int power;
    public Sprite icon;
}
```

このようにして作ったScriptableObjectは、Projectビューにアセットとして保存され、複数のGameObjectから参照できます。

🔍 まとめ
- MonoBehaviourは「動きや振る舞い」を定義する。
- ScriptableObjectは「データや設定」を定義する。
両者を組み合わせることで、柔軟で保守性の高いゲーム設計が可能になります。
もし「ScriptableObjectを使ったデータ駆動設計」や
「アセットバンドルとの連携」などに興味があれば、さらに深掘りできますよ！


---

## 📦 Prefabとは？
- GameObjectをテンプレート化したものです。
- Projectビューに保存されていて、何度でも使える再利用可能なオブジェクトです。
- Prefabを使うことで、同じ構造のGameObjectを簡単に複製・管理できます。

以下に使われる。
- UIパーツ（ボタン、パネルなど）
- 弾やエフェクト
- NPCやアイテム
- 建物や地形のパーツ

🧱 Prefabの特徴
- 再利用可能：一度作成すれば、何度でもシーンに配置できます。
- 編集が一括反映：Prefabの元を編集すると、すべてのインスタンスに変更が反映されます。
- 管理が簡単：複雑なオブジェクト構成でも、1つのファイルとして管理できます。
- 動的生成が可能：スクリプトから Instantiate() を使って、Prefabを動的に生成できます。

---
---

## 🧬 Unityオブジェクトのライフサイクル
1. 生成（Instantiation）
- Instantiate() メソッドを使って、Prefabや既存のGameObjectを複製してシーンに配置します。
- この時点で、GameObjectはアクティブ状態でシーンに存在し、各コンポーネント（Transform, Renderer, Scriptなど）が初期化されます。

2. Awake()
- スクリプトがアタッチされている場合、Awake() メソッドが最初に呼ばれます。
- 他のコンポーネントとの依存関係を設定したり、初期化処理を行うのに適しています。
- オブジェクトがアクティブでなくても呼ばれます。

3. OnEnable()
- GameObjectがアクティブ状態になると呼ばれます。
- UIの表示やイベント登録など、アクティブ時に必要な処理を記述します。

4. Start()
- Awakeの後、最初のフレームの前に一度だけ呼ばれます。
- 他のオブジェクトとの連携が必要な初期化処理に適しています。

5. Update() / FixedUpdate() / LateUpdate()
- 毎フレーム呼ばれるメソッド。
- ゲームロジック、物理演算、カメラ追従などをここで処理します。

6. OnDisable()
- GameObjectが非アクティブになると呼ばれます。
- イベントの解除や一時停止処理などに使います。

7. OnDestroy()
- Destroy() メソッドでオブジェクトが破棄されるときに呼ばれます。
- メモリ解放、イベント解除、リソースのクリーンアップなどを行います。
