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

function stopCamera() {
    console.log("Camera Stop");
    if (localVideo.srcObject) {
        try {
            localVideo.srcObject.getTracks().forEach(
                track => { track.stop(); }
            );
        }
        catch (error) {
            console.error(error);
        }
    }
}

stopButton = document.getElementById('stop');
stopButton.onclick = stopCamera;