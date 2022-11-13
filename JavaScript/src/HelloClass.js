class Animal{
    constructor (baw) {
         this.baw = baw;
         this.baw2 = "non";
        }
    // コンストラクタは2つは作れないっぽい(知らんけど、)
    /*constructor (baw,baw2) {
        this.baw = baw;
        this.baw2 = baw2;
   }*/
   say (){
        console.log(this.baw+ " " + this.baw2);
    }
}

// new をつけないとコンストラクタが動かないらしい
cat = new Animal("mew mew");
// dog = new Animal("baw","wow");

cat.say();
//dog.say();

// lambda
const f = function(name){
    console.log("hello "+ name+ "!");
};

function hello(name){
    console.log("hello "+ name);
};

arrowLambda = (num) => {
    return num * 2
  }
  
f("Jhon");
hello("Tom");
arrowLambda(3);

// 🌟継承はこんな感じ
class fish extends Animal{
    constructor(name)
    { 
        //親クラスのコンストラクタはこんな感じ
        super("I`m fish!");
        super.baw2 ="";
    }

    talk(){
        // 親クラスの関数を実行する。
        super.say();
    }
}

jerryfihs = new fish("");
jerryfihs.talk();