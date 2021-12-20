

// *******************************************
// Leaflet共通処理
// 地図表示、アイコン表示、表示アイコン初期化
// ※注意書き
// 　オープンストリートマップを使っている(国土地理院を使う場合はリンク先を変更する必要あり)
//   varになっているmapObj、putIconsObjについては、画面上で操作する必要あり
// *******************************************

// マップオブジェクト
var mapObj = null;
// 地図のリンク先
const MAP_LINK = '<a href="http://openstreetmap.org">OpenStreetMap</a>';
// 地図のタイルＵＲＬ
const MAP_TILE = 'http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png'

// 初期のズーム値(小さいほうが引き)
const START_ZOOM = 15;
// 最大のズーム値
const MAX_ZOOM = 20;
// 最大のズーム値
const ICON_FOLDER = "";
// アイコンのファイルパス定義
const ICON_IMAGE = {
    0: "current.jpg",
    1: "point.jpg",
}
// アイコンのサイズ
const ICON_SIZE = 36;
// アイコンオブジェクト
var putIconsObj = [];
// 現在地アイコンオブジェクト
var currentIconObj = [];


// --------------------------------------------
// 地図表示関数
// cur_lat_t : 表示する中心緯度
// cur_lng_t : 表示する中心経度
// --------------------------------------------
const viewMap = function(cur_lat_t, cur_lng_t){

    // マップオブジェクトに設定現在地と初期ズーム値を設定
    mapObj = L.map('map_area').setView([cur_lat_t, cur_lng_t], START_ZOOM);
    // 地図表示情報を設定
    L.tileLayer(
        MAP_TILE,
        {
            attribution: 'Map data &copy; ' + MAP_LINK, // リンク元
            maxZoom: MAX_ZOOM                           // 最大ズーム値
        }
    ).addTo(mapObj);
};

// --------------------------------------------
// １つのアイコンを表示する
//  lat_t:表示緯度
//  lng_t:表示経度
//  imgIdx:アイコン画像のインデックス(ICON_IMAGE参照)
//  popup_value:ポップアップ表示内容
//  iconSize:アイコンのサイズ / 形式は [横, 縦] で指定
// --------------------------------------------
const putIcon = function(lat_t=0.0, lng_t=0.0, imgIdx=0, popup_value=null, iconSize=[ICON_SIZE, ICON_SIZE]){

    // アイコン情報を設定取得
    var pointIcon = L.icon({
        iconUrl: ICON_FOLDER + ICON_IMAGE[imgIdx], // 画像のＵＲＬ
        iconSize: iconSize,                        // アイコンサイズ
    });

    // 取得するアイコンオブジェクト
    let iconObj = null;
    // アイコン設定
    iconObj = L.marker([lat_t, lng_t], {icon: pointIcon}).addTo(mapObj);
    // ポップアップがある場合
    if(popup_value){
        // ポップアップ内容をアイコンに設定
        iconObj.bindPopup(popup_value);
        // アイコンにポップアップイベントを設定
        iconObj.on("click", function(){
            this.openPopup();
        });
    }
    return iconObj;
};

// --------------------------------------------
// 地図上のアイコンを初期化(アイコンを再表示する際などに使用)
// --------------------------------------------
const dropIcons = function(){
    if(putIconsObj.length){
        // 地図上のアイコンを初期化
        for(idx in putIconsObj){
            // 地図上から表示を外す
            mapObj.removeLayer(putIconsObj[idx]);
            // ポップアップイベントをオフにする
            putIconsObj[idx].off("click");
        }
    }
    // アイコンオブジェクト配列を初期化
    putIconsObj = [];
}
