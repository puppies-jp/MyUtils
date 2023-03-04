# Javaを動かす

[JSP(tomcat)を使ってみる](Jsp)

## ubuntu環境構築

```bash
apt install openjdk-8-jre
apt install openjdk-8-jdk
```

---
---

## Hello Worldする

```java
// 🌟ファイル名を定義したクラスと同じにすること!!
// ビルドできなくなる
public class HelloWorld{
    // 🌟 classを実行するため、staticなメイン関数がいる
   public static void main(String[] args){
     System.out.println("Hello World!!");
   }
}
```

```bash
# 🌟 コンパイルする(HelloWorld.classが作成される。)
javac HelloWorld.java 

# 🌟実行(classファイルを実行する)
java HelloWorld
```
