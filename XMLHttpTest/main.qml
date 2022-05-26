import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Component.onCompleted: test()

    function test() {
        var xhr = new XMLHttpRequest();
//        var url = "http://34679y496m.oicp.vip/download/version.json"
        var url = "http://34679y496m.oicp.vip/download/Sirius-Pro 4.0_beta_5_23.rar"

        xhr.onreadystatechange = function() {
            /*
             * XMLHttpRequest 状态解析：
             * 0 - UNSENT(初始状态，未打开)，此时 xhr 对象被成功构造，open()方法还未被调用
             * 1 - OPENED(已打开，未发送)，open()方法已被成功调用，send()方法还未被调用。
                   注意：只有处于 OPENED 状态，才能调用 v.setRequestHeader() 和 xhr.send() 否则会报错
             * 2 - HEADERS_RECEIVED(已获取响应头)，send()方法已经被调用, 响应头和响应状态已经返回
             * 3 - LOADING (正在下载响应体)，响应体 (response entity body) 正在下载中，
                   此状态下通过 xmlhttp.response 可能已经有了响应数据
             * 4 - DONE (整个数据传输过程结束)，整个数据传输过程结束，不管本次请求是成功还是失败
             */
            if ( xhr.readyState === XMLHttpRequest.HEADERS_RECEIVED ) {
                console.log("Response Headers -->");
                console.log(xhr.getAllResponseHeaders() + "\n");
            }

            else if ( xhr.readyState === XMLHttpRequest.DONE ) {
                console.log("status：" + xhr.status)
                console.log("responseText: " + xhr.responseText)
            }
        }

        xhr.open("GET", url, true);
        xhr.send();
    }
}
