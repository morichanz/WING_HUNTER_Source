[WING HUNTER]（ウィングハンター）

[製作者]
 森田 弘之介（もりた こうのすけ）

[製作期間]
 2022/06/21 ～ 2022/09/30

[開発環境]
 OS : Windows10 Home
 ツール : VisualStudio 2019
 言語 : C++17
 ライブラリ : 
　　OpenGL 4.5
　　GLFW 3.3.6

[ゲーム内容]
 回復、強化無しの完全プレイヤースキルを試す縦スクロールシューティングゲームです。
 自分の残機が０の状態でHPが０になるとゲームオーバーです。
 全６ステージで、各ステージの最後にはボスが待ち構えています。
 ボスを撃破するとステージクリアとなり、次のステージへ進みます。
 全ステージをクリアすると、クリア時のスコアに応じてS～Cまでのランクが表示されます。
 ※ゲームオーバーの場合、その時のスコアとランクが表示されます。

[制作目的]
「神威」というシューティングゲームを参考に、高低差の表現や、
 ボスが迫力のあるレーザーを撃つといった演出を再現したくて制作しました。
 再現しようと思った理由は、中ボスが出現する際、大きく出てくる演出に「デカッ！」っと
 驚き、衝撃を受けたからです。
 また、ボスとの戦闘時には、自機が覆いかぶさるサイズのレーザービームが2つ同時に発射される
 攻撃があり、非常にかっこいいと思ったのも理由の一つです。

[工夫した部分]
 ・イージング
　敵の移動に抑揚をつけたくて、EaseOutExpoなどいくつかのイージング関数を実装しました。
　これによりゲームらしい動きになりました。
　また、イージング関数をまとめたクラスを作ることにより、イージング関数をほかのクラスでも
　自由に使用できるようになりました。

 ・コンフィグでの設定
　コンフィグ画面では、キー設定、音量設定が簡単に設定できるようになっています。
　設定した内容はJsonファイルに保存しているのでゲームをやめても設定した内容は残ります。
　また、ゲーム画面では設定したキーが左側のoperationで反映されるようになっています。

 ・status関係をJsonで管理
　元々プログラム内で設定内容を記入しており、設定を変更する度に設定内容が書いてあるクラスを
　行き来しないといけない状況でした。
　しかし、Jsonファイルを利用することで、変更内容を項目ごとに一括して管理ができるので、
　非常に見やすく、管理が楽になりました。

 ・全ステージ及びボス戦のBGMの変更
　私のシューティングゲームは基本的な動きは同じなので、ボスの見た目や移動パターンを変えた
　だけでは今一つステージごとの差別化が足りないと感じていました。
　そこで、ボスの攻撃、ボスの行動パターン以外で差別化を図ろうと思ったとき、アンダーテール
　を思い出しました。アンダーテールのボスBGMは毎回違っており、それによりボスの印象を強くし
　ています。

 ・レーザー
　神威というシューティングゲームにあるレーザーを実現したくて実装しました。レーザーを発射
　するときは一度動きを止め、チャージを行います。その後、まっすぐ発射しながら移動します。
　急にレーザーを撃つのではなく、チャージ時間を設けることでボスに攻撃を続けるのか、または
　早めに避けるのかをプレイヤーに選択させます。
　これにより「駆け引き」が生まれ、楽しみ方が増えます。

 ・大型機
　神威の中ボスであった演出で初めに大きく見せて移動しながらサイズを小さくすることで中ボス
　の登場時のインパクトを大きく見せていました。この演出を使いたくて大型機の出現方法を移動
　プラスZ軸の変更によって表現しました。
　この表現によりリアル感が出てよりゲームらしさが出たと実感しています。

 ・ボスの移動パターンをランダム化
　ボスの動きとして、完全ランダムで中心から移動が始まり中心に戻ってくるという特徴があります。
　必ず動き始めが中心で、動き終わりも中心に戻ってくるようになっているので、３～４パターンある
　行動をランダムで使用しても問題なく動きます。

 ・移動やショットの説明をわかりやすく！
　ショットや移動がわかりやすくなるように左側に表示し、どのキーを押しているのかがわかるように
　動きも追加しました。

 ・HPゲージでダメージをわかりやすく！
　HPゲージもただ減らすだけでなく、ダメージを受けたことがわかるように「ブルッ」と振動を加えて
　ダメージを受けたことを認知しやすい工夫をしています。

[ファイル構成]
 WINGHUNTER.zip：ゲーム本体
 WINGHUNTER_Source.zip：ゲームのソースコード
 プレイ動画.mp4：ゲーム紹介動画
 readme.txt：このファイル

[ソースファイル構成]
WINGHUNTER
+- ImGui				ImGuiライブラリ
+- packages				NuGetパッケージ
+- data					status保存データファイル(json)
+- Res
|   +- 3Dobj				3Dobjファイル 
|   +- Audio				音声データ
|   +- DebugBoss			デバッグ用ボスマップデータ
|   +- Font				文字のフォントフォルダ
|   |   +- StageFont			ステージごとのフォント
|   |   +- TitleAnim			タイトルロゴのフォント
|   +- Images				タイトル、フレーム、ゲームオーバー、クリア、その他オブジェクトの画像
|   +- LoadAnim				ロード画面の時計
|   +- PlayerHp				プレイヤーのHP及び残機の画像
|   +- Rank				クリア時のスコアランク(S～B)
|   +- Stage				ステージデータ
|   +- *.frag				フラグメントシェーダ
|   +- *.vert				頂点シェーダ
+- Src
|   +- Audio				オーディオフォルダ
|   |   +- AudioEvent.h			オーディオ再生インターフェイス
|   |	+- AudioSetting.h		オーディオ再生プレイヤーインターフェイス
|   |   +- EasyAudio.cpp		オーディオ設定インターフェイス
|   |   +- Easyaudio.h			オーディオ設定インターフェイス
|   +- Component			コンポーネントフォルダ
|   |   +- Animator			アニメーターフォルダ
|   |   |   +- Animator2D.h		アニメーション再生コンポーネント
|   |	+- Boss				ボスフォルダ
|   |   |   +- Beam.h			ビーム移動管理コンポーネント
|   |	|   +- BeamDamageSource     	ビームのダメージ管理コンポーネント
|   |   |   +- BossAction		ボス管理コンポーネント
|   |	|   +- BossBulletExPlosion  	ボスの弾の爆発管理コンポーネント
|   |	|   +- BossExplosion		ボスの爆発管理コンポーネント
|   |	|   +- BossHealth.h		ボスのHP管理コンポーネント
|   |	|   +- BossShadowMove.h		ボスの影管理コンポーネント
|   |	|   +- BossWeaponAction		ボスの武器管理コンポーネント
|   |	+- Bullet			弾フォルダ
|   |	|   +- BoundBullet.h		跳ね返る弾の管理コンポーネント
|   |	|   +- BulletMove.h		弾の移動管理コンポーネント
|   |	|   +- CircleBullet.h		円状の弾管理コンポーネント
|   |	|   +- FireBullet.h		通常弾管理コンポーネント
|   |	+- Enemy			敵フォルダ
|   |	|   +- LargeEnemyAction.h	大型機管理コンポーネント
|   |	|   +- MiddleEnemyAction.h	中型機管理コンポーネント
|   |	|   +- SmallEnemyAction.h	小型機管理コンポーネント
|   |	+- Score			スコアフォルダ
|   |	|   +- Score.h			スコア加算コンポーネント
|   |	|   +- ScoreManager.h		スコア管理コンポーネント
|   |	+- Color.h			ゲームオブジェクトの色管理コンポーネント
|   |	+- DamageSource.h		ゲームオブジェクトのダメージ管理コンポーネント
|   |	+- Explosion.h			爆発管理コンポーネント
|   |	+- Health.h			ゲームオブジェクトのHP管理コンポーネント
|   |	+- Move.h			ゲームオブジェクトの移動コンポーネント
|   |	+- OutOfScreen.h		画面外と判定されたオブジェクトの管理コンポーネント
|   |	+- ScaleManager.h		ゲームオブジェクトのサイズ管理コンポーネント
|   |	+- Text.h			テキスト管理コンポーネント
|   +- Engine				エンジンフォルダ
|   |   +- Engine.cpp			エンジンクラス
|   |   +- Engine.h			エンジンクラス
|   +- FramebufferObject
|   |   +- FramebufferObject.cpp
|   |   +- FramebufferObject.h
|   +- GameObject			ゲームオブジェクトフォルダ
|   |   +- GameObject.cpp		ゲームオブジェクトクラス
|   |   +- GameObject.h			ゲームオブジェクトクラス
|   +- glad				GLADライフラリ
|   +- json				jsonライブラリ
|   +- Mesh				メッシュフォルダ
|   |   +- Mesh.cpp			メッシュ管理クラス
|   |   +- Mesh.h			メッシュ管理クラス
|   +- ProgramPipeline			プログラムパイプラインフォルダ
|   |   +-ProgramPipeline.cpp		プログラムパイプライン管理クラス
|   |   +- ProgramPipeline.h		プログラムパイプライン管理クラス
|   +- Scene				シーンフォルダ
|   |   +- ClearScene.cpp		クリアシーン管理クラス
|   |   +- ClearScene.h			クリアシーン管理クラス
|   |   +- Fade.cpp			フェード管理クラス
|   |   +- Fade.h			フェード管理クラス
|   |   +- GameOverScene.cpp		ゲームオーバーシーン管理クラス
|   |   +- GameOverScene.h		ゲームオーバーシーン管理クラス
|   |   +- LoadScene.cpp		ロードシーン管理クラス
|   |   +- LoadScene.h			ロードシーン管理クラス
|   |   +- MainGameScene.cpp		メインゲームシーン管理クラス
|   |   +- MainGameScene.h		メインゲームシーン管理クラス
|   |   +- Scene.h			シーン管理クラス
|   |   +- TitleScene.cpp		タイトルシーン管理クラス
|   |   +- TitleScene.h			タイトルシーン管理クラス
|   +- Sprite				画像データフォルダ
|   |   +- Sprite.h			画像データ管理クラス
|   |   +- SpritePriority.h		画像データの表示優先順位管理クラス
|   +- Texture				テクスチャフォルダ
|   |   +- Texture.cpp			テクスチャ管理クラス
|   |   +- Texture.h			テクスチャ管理クラス
|   +- VecMath				ベクトルファイル
|   |   +- VecMath.cpp			ベクトルの計算管理クラス
|   |   +- VecMath.h			ベクトルの管理クラス
|   +- Easing.h				イージング管理クラス
|   +- main.cpp				描画ウィンドウの作成クラス
|   +- Rect.h				衝突判定用長方形クラス
|   +- TileMap.h			タイルの配置クラス
+- imgui.ini				ImGui構成設定ファイル
+- WING_HUNTER.sln			VSソリューションファイル
+- WING_HUNTER.vcxproj			VSプロジェクトファイル
+- WING_HUNTER.vcxproj.filters		VSフィルタファイル
+- WING_HUNTER.vcxproj.user		VSユーザー設定ファイル
+- packages.config			NuGetパッケージ設定ファイル

[インストール方法]
 WINGHUNTER.zipを任意のフォルダに展開してください。

[実行方法]
 展開した「WING_HUNTERフォルダ」にある「WING_HUNTER.exe」をダブルクリックするとゲームが起動します。

[遊び方]
 キーボード：
　W/A/S/D ---------- 自機の移動
　Enter ------------ 決定
　Space ------------ 弾を発射（押しっぱなしで自動連射）

 Config:
  音量、キーバインドを設定できます。

 IMGUIの表示：
  G+U+Iキー同時押しで開けます。

[使用素材]
 SE・BGM：https://dova-s.jp/
 Enemy画像：UnityAsset
 ロゴ: https://cooltext.com
 2D素材：https://github.com/tn-mai/OpenGL3D2022/blob/master/res/objects.png
         https://assetstore.unity.com/packages/2d/characters/2d-pixel-spaceships-2-small-ships-explosions-133395
　　　　 https://assetstore.unity.com/packages/2d/environments/2d-spaceships-free-trial-146307
         https://assetstore.unity.com/packages/2d/characters/2d-pixel-spaceship-two-small-ships-131545

[参考資料]
 イージング：https://easings.net/j