# React 画面遷移

Reactで画面遷移をする方法についてまとめる

## パラメータなしのページを追加する

appディレクトリの下にHelloディレクトリを作成し、その中にpage.tsxを作成する。
HelloディレクトリがURLのパスになる。
デフォルトでHelloディレクトリの下のpage.tsxがURLにアクセスしたときに表示される。

```js
# /app/Hello/page.tsx
export default  function Hello() {
    return (<div>Hello this is  New Page</div>);
}
```

## URLに変数をつけて実行する1

※ React 16以降であれば、以下のようにしてURLに変数をつけて実行することができる。

1. appディレクトリの下にHelloディレクトリを作成
2. app/Hello/[name]ディレクトリを作成する。

* 送信側

```js
import Link from "next/link";

export default function UserList() {
    const users = [
        { id: 1, name: "Taro" },
        { id: 2, name: "Hanako" },
    ];

    return (
        <ul>
            {users.map((user) => (
                <li key={user.id}>
                    // LinkコンポーネントのhrefにURLの変数を指定することで、URLに変数をつけて遷移することができる
                    <Link href={`/Hello/${user.name}`}>{user.name} </Link>
                </li>
            ))}
        </ul>
    );
}
```

* 受信側

```js

// app/Hello/[name]/page.tsx
type Props = {
    //# URLの変数はparamsで受け取ることができる
    // 変数はPromiseで受け取る必要があり、urlの変数とパラメータの名前は同じにする必要がある
    params: Promise<{ name: string }>;
}

// URLの変数を受け取るために、propsでparamsを受け取る必要があることに注意
export default async function Hello({ params }: Props) {
    // paramsからnameを受け取るが、変数名をname以外にすることができない
    const { name } = await params
    return (<div>Hello {　name　} this is  New Page</div>);
}
```

## URLに変数をつけて実行する2(クエリパラメータ編)

* 送信側

```js
import Link from "next/link";

export default function UserList() {
    const users = [
        { id: 1, name: "Taro" },
        { id: 2, name: "Hanako" },
    ];

    return (
        <ul>
            {users.map((user) => (
                <li key={user.id}>
                    <Link
                    // hrefにpathnameとqueryを指定することで、
                    // クエリパラメータをつけて遷移することができる
                        href={{
                        pathname: "/Hello",
                        query: {
                            name: user.name ,
                            id: user.id,
                        }
                        }}>
                     Id.{user.id}/{user.name}のページへ
                    </Link>
                </li>
            ))}
        </ul>
    );
}
```

* 受信側

```js
'use client'
import { /*useRouter,*/ useSearchParams } from "next/navigation"

// サンプルのため引数がないが、/app/Hello/[name]/page.tsx と、
// URLの変数をクエリパラメータで受け取る方法と両立できる点に注意
export default function Hello() {

    //ルーティングの設定
    // const router = useRouter();
    //クライアントコンポーネントフックであるuseSearchParamsを使用する
    const searchParams = useSearchParams();
    const name = searchParams.get('name');
    const id = searchParams.get('id');

    return (<div>Hello Id:{id}/{name} this is  New Page</div>);
}
```
