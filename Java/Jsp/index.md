# Java/Jsp

WebサーバがリアルタイムにHTMLページを組み立て、Webブラウザに送り返すという仕組み。
JSPでは、通常のHTMLページの代わりに、「.jsp」という拡張子を持つ「JSPページ」を記述することで、Webコンテンツを動的に生成できます

## tomcat フォルダ構成

```sh
user@macbook webapps % tree ./helloworld 
# 🌟HTMLファイル、イメージファイル、JSPファイルなどを格納
# ディレクトリは自由に作成できます
/usr/local/tomcat/webapps/helloworld/ # 🌟webアプリルートディレクトリ
├── WEB-INF # 🌟Webアプリケーション環境定義ファイル(web.xml)を格納
│   ├── classes # 🌟サーブレットなどのJavaのクラスファイルを格納
│   │   └── xxx.class
│   └── lib # 🌟JavaクラスファイルをJAR形式のJARファイルを格納
│   └── web.xml
└── index.jsp

3 directories, 3 files
```

## XMLファイル書き方

```xml
<?xml version="1.0"?>
<web-app>
    <servlet>
        <!-- 🌟サーブレット・クラスsample.TestServlet定義 -->
        <servlet-name>TestServlet</servlet-name>
        <servlet-class>sample.TestServlet</servlet-class>
    </servlet>
    <servlet-mapping>
        <!-- 🌟sample.TestServletに/testというURLを割り当て -->
        <servlet-name>TestServlet</servlet-name>
        <url-pattern>/test</url-pattern>
    </servlet-mapping>
</web-app>
```

## サンプル1(for loop)

```jsp
// sample1.jsp
<%@ page contentType="text/html; charset=Windows-31J" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<html>
  <head>
    <title>JSPサンプル</title>
  </head>
  <body>

    <!-- 🌟こうすることで、よくあるforループができる -->
    <c:forEach var="item" begin="1" end="10">
        ${item}
    </c:forEach>

  </body>

  <script type="text/javascript">
 
  <!-- 🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟 -->
  <!-- 🌟script内に書き込むことで、jsのデータを書き込むこともできる -->
  let data = {
  <%
  for(int i=0;i<10;i++){
    if (i !=0) out.println(",");
      out.println( "data" +i + ":" + i);
    }
    %>
  };
  <!-- 🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟🌟 -->
</script>

</html> 
```

## サンプル2

```jsp
// pageディレクティブ
// <%@ このタグで JSPページの文字コードやコンテンツ内容をTomcatに伝える機能を持つ
<%@ page contentType="text/html; charset=Shift_JIS" %>
<jsp:useBean id="now" class="java.util.Date" />

//取得したいインスタンスのクラスをインポートする
<%@ page import = "bean.Human" %>

//リクエストスコープ(cookie)からインスタンスを取得する
<%　Human h = (Human) request.getAttribute("human"); %>

<html>
  <head>
    <title>JSPサンプル</title>
  </head>
  <body>
    //インスタンスのプロパティにアクセスし、値を取り出す
    <%= h.getName() %>さんの年齢は<%= h.getAge() %>歳です
    こんにちは、今の日時は <%= now %> です。

    // 式: Java式の実行とWebブラウザへの出力
    <%= new java.util.Date() %>
    // スクリプトレット: Javaコードの実行
    <% Date d = new Date(); %>
    // 宣言: Javaフィールドまたはメソッドの宣言
    <%! Date d = null; %>
    // コメント
    <%-- コメント --%>
  </body>
</html>
```

## サーブレットでセッション(Cookie)を使う

```java
Human human = new Human();
human.setName("かずきち");
human.setAge(29);

//HttpSessionインスタンスの取得（javax.servlet.ttp.HttpSessionをインポートしておく必要がある）
HttpSession session = request.getSession();

//セッションスコープにインスタンスを保存
// 🌟クラスをセットしていることに注目!!
// jsp側からgetで取得することもできる
session.setAttribute("human", human);

//セッションスコープからインスタンスを取得
Human h = (Human) session.getAttribute("human");
```
