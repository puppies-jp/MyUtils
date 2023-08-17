# ReactNative

ReactNativeを使ってみる
触った感じの所感、`index.js`でキックされるみたい。
んで、コンポーネントの登録で画面を表示する。
なので、コンポーネントを継承したクラスをjsの別ファイルで作成し、index.jsなどの適切なタイミングで呼べば画面を変えれる

画面遷移を使いたたい場合、以下のようにして`react-navigation`を使う。
React(Nativeじゃない)でも`navigation`で遷移が作られる

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
            <View
                style={{ flex: 1, justifyContent: "center", alignItems: "center" }}
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
