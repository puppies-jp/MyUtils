const { createServer } = require("http");
const { Server: FileServer } = require("node-static");
const { Server: WebSocketServer } = require("ws");
const getLocalIp = require("./utils/getLocalIp");

/*
 *  node server.js
 */

const PORT_NUM = 8080;

(async () => {
  // ローカルipアドレス取得
  const host = await getLocalIp().catch((err) => {
    throw err;
  });

  // httpサーバー立ち上げ
  const fileServer = new FileServer(`${__dirname}/public`);
  const server = createServer((request, response) => {
    request
      .addListener("end", function () {
        fileServer.serve(request, response);
      })
      .resume();
  });

  // websocketサーバー立ち上げ（兼用）
  const wsServer = new WebSocketServer({
    server: server,
  });

  // クライアントとの接続確立後
  wsServer.on("connection", (ws) => {
    ws.on("message", (message) => {
      console.log("Received: " + message);

      if (message === "hello") {
        ws.send("hello from server!");
      }
    });

    ws.on('close',function(){
        console.log('I lost a client');
    });
  });

  // サーバーリッスン
  server.listen(PORT_NUM, 'localhost');
  server.on("listening", () => {
    // 準備完了でコンソールにURL表示
    const addressInfo = server.address();
    console.log(`http://${addressInfo.address}:${addressInfo.port}`);
  });
})();

