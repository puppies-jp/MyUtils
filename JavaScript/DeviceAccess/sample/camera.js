var localVideo = document.getElementById('localVideo');
let mediaRecorder;
let recordedBlobs;

const types = [
    "video/webm",
    "audio/webm",
    "video/webm;codecs=vp8",
    "video/webm;codecs=daala",
    "video/webm;codecs=h264",
    "audio/webm;codecs=opus",
    "video/mpeg",
];

typeList = [];
for (const type of types) {
    canUse = MediaRecorder.isTypeSupported(type);
    console.log(`${type} に対応している? ${canUse ? "たぶん!" : "いいえ :("}`);
    if (canUse) {
        typeList.push(type);
    }
}


function handleDataAvailable(event) {
    if (event.data && event.data.size > 0) {
        recordedBlobs.push(event.data);
    }
}


function startRecording(stream) {
    recordedBlobs = [];
    const options = {
        //mimeType: 'video/mp4'
        mimeType: typeList[0]
    };
    try {
        mediaRecorder = new MediaRecorder(stream, options);
    } catch (error) {
        console.log(`Exception while creating MediaRecorder: ${error}`);
        return;
    }

    mediaRecorder.onstop = event => {
        console.log("Recorder stopped: ", event);
    };

    mediaRecorder.ondataavailable = handleDataAvailable;
    mediaRecorder.start(10);
    console.log("MediaRecorder started", mediaRecorder);
}

function openCamera() {
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
    mediaRecorder.stop();
    console.log("Recorded media.");
    const blob = new Blob(recordedBlobs, { type: "video/webm" });
    const url = window.URL.createObjectURL(blob);
    const a = document.createElement("a");
    a.style.display = "none";
    a.href = url;
    a.download = "rec.webm";
    document.body.appendChild(a);
    a.click();
    setTimeout(() => {
        document.body.removeChild(a);
        window.URL.revokeObjectURL(url);
    }, 100);
}

st = document.getElementById('state');
stopButton = document.getElementById('stop');
stopButton.onclick = (() => {
    stopCamera();
    st.innerText = "Stop";
});
startButton = document.getElementById('start');
startButton.onclick = (() => {
    startRecording(localVideo.srcObject);
    st.innerText = "Recording";
});
openButton = document.getElementById('openCamera');
openButton.onclick = (() => {
    openCamera();
    st.innerText = "Camera Running";
});
openButton.click();
