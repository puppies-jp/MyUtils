/*
npm init -y
npm i ws --save
*/
// 実行方法
// node index.js
// ブラウザで以下を開く
// open index.html
var server = require('ws').Server;
var s = new server({port:5001});
count =0;
connectCount = 0;

function sendMessage(client){

    // sendはtextなので、jsonをstrに変換して送信する。
    // 受信先でjsonに再度パースすることで戻す。
    // var jdata = JSON.parse(e.data);
    client.send(
        JSON.stringify(
            {
                "user":connectCount,
                "count":count,
                "Now":new Date()
            }
            )
        );
        count +=1;
}

s.on('connection',function(ws){
    connectCount += 1;

    ws.on('message',function(message){
        console.log("Received: "+message);

        s.clients.forEach(function(client){
            //client.send(message+' : '+new Date());
            // 🌟送信を2回行っても動く!!
            //client.send("Fuck you"+' : '+new Date());
            sendMessage(client);
        });
    });

    ws.on('close',function(){
        console.log('I lost a client');
        connectCount -=1;
    });

});

// 定期的にサーバーから送るには？
// -> 非同期でsendを呼び出すことでできる。
console.log("Loop running");
async function loops(){ 
    while(connectCount >=0){
        s.clients.forEach(
            function(client){
                sendMessage(client);
            }
        );
    
    await new Promise(s => setTimeout(s, 3000));
}
}
loops();
