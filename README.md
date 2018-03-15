# HDRImageViewer ver 0.4
HDR image viewer for HDR10 displa1y.

Windows 10のHDRに対応したHDR10ディスプレイでOpenEXRやJPEG XRやDDSを表示するサンプルです.

# Screen Shots

![ScreenShot](https://github.com/shaderjp/HDRImageViewer/blob/master/ScreenShots/screenshot.jpg)

ヒートマップモードでは0 nitsから10,000 nitsの輝度に応じたヒートマップの表示が行えます(ST.2084時).内部計算時の輝度の分布がわかります.

![ScreenShot](https://github.com/shaderjp/HDRImageViewer/blob/master/ScreenShots/heatmap.jpg)
ヒートマップモード

ヒートマップのテーブルは下記です.左が0 nitsで右が10,000 nitsです.

![ScreenShot](https://github.com/shaderjp/HDRImageViewer/master/ScreenShots/heatmapReference.png)

ヒートマップについては下記の記事のテーブルを参考にしています.

https://www.resetera.com/threads/hdr-games-analysed.23587/

※スクリーンキャプチャ時にsRGBに落ちています。

# Requirements
- Windows 10 SDK 10.0.16299.0
- Visual Studio 2017 with the Windows 10 Fall Creators Update SDK
- Direct3D12 Compatible GPU

# Building Sources

ThirdPartyに入っているサブモジュール.

- DirectXTex　https://github.com/Microsoft/DirectXTex
- imgui　https://github.com/ocornut/imgui

nugetより取得したライブラリ
- OpenEXR openexr-msvc14-x64 https://www.nuget.org/packages/openexr-msvc14-x64/
- zlib zlib-vc140-static-32_64 https://www.nuget.org/packages/zlib-vc140-static-32_64/

nugetのパッケージはVisual Studio 2017メニューの【ツール】の【NuGet パッケージマネージャー】、【ソリューションのNuGetパッケージの管理】を開きます。

下図の【ソリューションのNuGetパッケージの管理】画面の右上の赤丸で囲った【復元】ボタンを押すことでダウンロードができます。

![NuGet Manager](https://github.com/shaderjp/HDROpenEXRViewer/blob/master/ScreenShots/nuget.jpg)

Releaseビルド済みの実行ファイルは下記にあります。

bin\D3D12HDRViewer.exe

# 操作方法

![ScreenShot](https://github.com/shaderjp/HDRImageViewer/blob/master/ScreenShots/screenshot.jpg)

## imguiのWindow

Windowはマウスで画面内で動かしたりサイズを変えたりできます。
初期状態ではWindowが重なった状態で出るかもしれません。

### Edit Window
- sRGB, ST.2084, Linear...色空間の変更。Linearは16bit colorで出力します.
- Load Fileボタン...ファイルの読み込み。OpenEXRとDDSに対応
- EV...EV値の変更+8.0 から -8.0
- Heatmap...ST.2084選択時にチェックを入れると輝度に応じたヒートマップが表示されます.

### Display Information

ディスプレイの輝度や色域の原色の位置（CIE 1931上の）などを確認できます。

正しい値が取得できない環境があります。

## キーボード操作
- PgUp,PgDn...色空間の変更
- H...10bitフォーマット時にST.2084とsRGBを切り替える
- U...GUIのオン、オフ。一度、閉じたimguiのWindowsを再表示する
- M...プリセットメタデータの変更
- Alt + Enter...フルスクリーン

###  サンプルデータ

以下のリポジトリにOpenEXR画像をいくつか公開しています

https://github.com/shaderjp/OpenEXRSampleImages

サンプル画像については下記の記事に書いています。

http://masafumi.cocolog-nifty.com/masafumis_diary/2018/01/hdr10openexrhdr.html

# Todo
- ベースとなるMicrosoftのサンプルコードから不要な処理が除去。
- English documentation

# 既知の不具合
- フルスクリーン時にファイルを開こうとするとダイアログ表示ができません。
- ファイルロードのダイアログで読み込むファイルタイプのフィルタを変えるとフィルタが反映されないことがある。フィルタが反映されないときはダイアログのフォルダ階層を移動してみてください。
- HDRがオンの際にLinear選択をするとimguiの輝度が高い状態になる。
