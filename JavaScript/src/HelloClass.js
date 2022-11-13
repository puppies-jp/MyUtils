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
