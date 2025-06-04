let Vdlst = document.getElementById("VideoTagList");
let start = document.getElementById("start");
let stat = document.getElementById("Status");
let count = 0;

function listenForClicks() {
    start.click();
};

sendEvent = { cmd: ["Start", "Stop"] };

document.addEventListener("click", (e) => {

    function send(tabs) {
        //コンテンツスクリプト側にメッセージを送る
        console.log("Send Message :" + e.target.textContent);
        browser.tabs.sendMessage(tabs[0].id, { cmd: e.target.textContent });
    }

    browser.tabs
        .query({ active: true, currentWindow: true })
        .then(send);
}
);

//コンテンツスクリプトからメッセージを受け取った時の処理。
browser.runtime.onMessage.addListener(function (message) {
    console.log("cmd:" + message.cmd + " text:" + message.text);

    stat.innerText = message.state;
    if (message.cmd == "Start")
        Vdlst.innerHTML = message.text;
});

function reportExecuteScriptError(error) {
    console.error(error);
}

// 
console.log("load");
browser.tabs
    .executeScript({ file: "/js/listUp.js" })
    .then(listenForClicks)
    .catch(reportExecuteScriptError);


