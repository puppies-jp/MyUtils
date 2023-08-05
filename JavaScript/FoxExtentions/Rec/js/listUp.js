
var recordedBlobs;
let mediaRecorder;
var state = "";
mediaMap = {};

function getType() {

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
        console.log(`${type} に対応している? ${MediaRecorder.isTypeSupported(type) ? "たぶん!" : "いいえ :("}`);
        if (canUse) {
            typeList.push(type);
        }
    }
    return typeList;
}

function handleDataAvailable(event) {
    if (event.data && event.data.size > 0) {
        recordedBlobs.push(event.data);
    }
}

function recordStart(stream) {
    recordedBlobs = [];
    options = {
        //mimeType: 'video/mp4'
        mimeType: getType()[0]
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

function recordStop() {
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

// search Tag
function getMediaTag(doc) {
    temp = {};
    try {
        temp = doc.getElementsByTagName('video');
    } catch (err) {
        console.log(err);
    }
    Array.from(temp,
        elm => {
            mediaMap[elm.id] = elm;
        }
    )
}

function getIframeMediaTag(doc) {
    console.log("Get Iframe");
    temp = {}
    try {
        temp = doc.getElementsByTagName("iframe");
    } catch (err) {
        console.log(err);
    }
    Array.from(temp,
        elm => {
            getMediaTag(elm.contentWindow.document);
        })
}

function getMediaElment(doc) {
    console.log("Get Element");
    getMediaTag(doc);
    getIframeMediaTag(doc);
}

console.log("Execute!!");
//getMediaElment(document);
function SendTag() {
    try {
        getMediaElment(document);
    } catch (error) {
        console.log(error);
    }
    console.log(mediaMap);
    _text = "";
    for (key in mediaMap) {
        _text += "<option>" + key + "</option>"
    }
    console.log("Send:" + _text);
    return _text;
}

var beforeTag = "";
var beforeStyle = "";

//バックグラウンド側からのメッセージを受け取ったとき。
browser.runtime.onMessage.addListener(function (msg) {
    try {
        console.log("Command:" + msg.cmd)
        switch (msg.cmd) {
            case "Start":
                state = "loaded";
                break;
            case "Rec":
                console.log(mediaMap[beforeTag].srcObject);
                console.log(mediaMap[beforeTag].src);
                // new MediaStream()
                recordStart(mediaMap[beforeTag].srcObject);
                state = "Recording";
                break;
            case "Stop":
                recordStop();
                state = "Stop";
                break;

            default:
                if (msg.cmd in mediaMap) {
                    if (beforeTag != "") {
                        mediaMap[beforeTag].style.border = beforeStyle;
                    }
                    beforeTag = msg.cmd;
                    beforeStyle = mediaMap[msg.cmd].style.border;
                    mediaMap[msg.cmd].style.border = "5px solid red";
                }
        }
        browser.runtime.sendMessage({
            cmd: msg.cmd,
            text: SendTag(),
            state: state
        });

    } catch (err) {
        console.error(err);
    }

});