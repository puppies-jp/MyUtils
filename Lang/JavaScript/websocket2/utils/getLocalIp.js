/**
 * ローカルipアドレスを取得。非同期
 */
module.exports = function () {
    return new Promise((resolve, reject) => {
      require("dns").lookup(require("os").hostname(), (
        error,
        address
      )=> {
        if (error) {
          reject(error);
          return;
        }
        resolve(address);
      });
    });
  };
  