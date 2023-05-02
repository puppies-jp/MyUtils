# ブラウザからカメラなどのデバイスにアクセスする

`navigator.mediaDevices.getUserMedia`を使う

- sample

```js
var localVideo = document.getElementById('localVideo');

if (navigator.mediaDevices.getUserMedia) {
    // window.streamは過去のstreamをプールするためのオブジェクト
    if (window.stream) {
        // 既存のストリームを破棄
        try {
            window.stream.getTracks().forEach(track => {
                track.stop();
            });
        } catch (error) {
            console.error(error);
        }
        window.stream = null;
    }

    // カメラとマイクの開始
    const constraints = {
        audio: true,
        video: true
    };
    navigator.mediaDevices.getUserMedia(constraints).then(stream => {
        // window.streamは過去のstreamをプールするためのオブジェクト
        // 削除とかに使われる
        window.stream = stream;
        localVideo.srcObject = stream;
    }).catch(e => {
        alert('Camera start error.\n\n' + e.name + ': ' + e.message);
    });

}
else {
    alert('Your browser does not support getUserMedia API');
}
```

## getUserMediaで扱えるメディアについて

`audio`,`video`のみらしい、、、

[constraintsパラメータ詳細](https://developer.mozilla.org/ja/docs/Web/API/MediaDevices/getUserMedia)

```js
// 音声と動画の両方を要求
{ audio: true, video: true }

// 🌟1280x720 のカメラ解像度の設定
{
  audio: true,
  video: { width: 1280, height: 720 }
}

// 🌟最小解像度1280x720 の設定
// この解像度以上のカメラがない場合、返却されたプロミスは OverconstrainedError として拒否される
{
  audio: true,
  video: {
    width: { min: 1280 },
    height: { min: 720 }
  }
}

// 🌟ideal の値からみた最適値を取る
{
  audio: true,
  video: {
    width: { min: 1024, ideal: 1280, max: 1920 },
    height: { min: 576, ideal: 720, max: 1080 }
  }
}
```

- フロント／リアカメラ,デバイスを指定して使う

```js
// 🌟フロントカメラ(利用できるなら)
{ audio: true, video: { facingMode: "user" } }
// 🌟リアカメラ(利用できるなら)
{ audio: true, video: { facingMode: { exact: "environment" } } }

// 🌟DeviceIDを指定して使う
// mediaDevices.enumerateDevices()で取得できる
{ video: { deviceId: myPreferredCameraDeviceId } }
{ video: { deviceId: { exact: myExactCameraOrBustDeviceId } } }

```
