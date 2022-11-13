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

arrowl = (num) => {
    return num * 2
  }
  
f("Jhon");
hello("Tom");
arrowLambda(3);