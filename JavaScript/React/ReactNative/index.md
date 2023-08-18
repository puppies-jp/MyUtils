# ReactNative

ReactNativeを使ってみる
触った感じの所感、`index.js`でキックされるみたい。
んで、コンポーネントの登録で画面を表示する。
なので、コンポーネントを継承したクラスをjsの別ファイルで作成し、index.jsなどの適切なタイミングで呼べば画面を変えれる

画面遷移を使いたたい場合、以下のようにして`react-navigation`を使う。
React(Nativeじゃない)でも`navigation`で遷移が作られる

- [staticな変数を定義する](#state)

- 位置情報を取得する `react-native-community/geolocation`
- Cameraを使う`react-native-vision-camera`

> 🚨 useState使い方によってレンダリングが遅い場合があるらしい。`React Developer Tools(Chrome拡張機能)`を使うことでレンダリングの状況を確認できる。
> 変数が新しくなったり、関数が新しくなったりすることで、無関係の部分までレンダリングされるためらしい、、
> `React.memo`,`useCallback`を使うことで緩和できたりする

```bash
npm install @react-navigation/native @react-navigation/native-stack
npm install react-native-screens react-native-safe-area-context

# pod(CoCoaPod)はパッケージの管理ツールのこと
# deintegrateでpodの管理を消してinstallで再インストール
cd ios
pod deintegrate
pod install
cd ..
```

```js
// home.js
import React, { Component, useState } from 'react';
import {
    Text,
    View,
} from 'react-native';

export class HomeScreen extends Component {
    render() {

        return (
            <View　style={{ flex: 1, justifyContent: "center", alignItems: "center" }}
            >
                <Text >Hello, world!</Text>
            </View>

        );
    }
}

export default HomeScreen;
```

```js

import { AppRegistry } from 'react-native';
import App, { HelloWorldApp } from './App';
import HomeScreen from './screen/home';
import { name as appName } from './app.json';
import { NavigationContainer } from '@react-navigation/native';
import { createBottomTabNavigator } from '@react-navigation/bottom-tabs';

const Tab = createBottomTabNavigator();

export function MyTabs() {
    return (
        <NavigationContainer>
            <Tab.Navigator>
                <Tab.Screen name="Home" component={HomeScreen} />
                <Tab.Screen name="Settings" component={HomeScreenX} />
                <Tab.Screen name="ScrollScreen" component={ScrollScreen} />
            </Tab.Navigator>
        </NavigationContainer>
    );
}

class temp extends Component {
    render() {
        return MyTabs()
    }
}

// NavigationContainerのrenderメソッドを持つクラスを渡すことで表示できる。
AppRegistry.registerComponent(appName, () => temp);
```

## <a name=state>stateについて</a>

> While you can think of props as arguments you use to configure how components render, state is like a component’s personal data storage. State is useful for handling data that changes over time or that comes from user interaction. State gives your components memory!

つまり、staticな変数のことみたい。
以下のように定義して使う。
(🚨なんか関数の中でしか定義できなかった。。。)

```js
function hoge(){
    //  [getter,setter] の関係となる。
    const [count, setCount] = useState(20);
    ...
    return(
        <Text>Home Screen Counter : {count}</Text>    
        <Button title="Count up Button!"
                onPress={() => setCount(count + 1)}
        />
    )
}
```
