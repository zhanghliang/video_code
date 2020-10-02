'use strict'//使用原始的js语法

//devices
var videoplay = document.querySelector('video#player');

//var audioplay = document.querySelector('audio#audioplay');
var audioSource = document.querySelector('select#audioSource');
var audioOutput = document.querySelector('select#audioOutput');
var videoSource = document.querySelector('select#videoSource');

//特效filter
var filtersSelect = document.querySelector('select#filter');

//截图picture
var snapshot = document.querySelector('button#snapshot');
var picture = document.querySelector('canvas#picture');

picture.width = 320;
picture.height = 240;

//媒体流约束信息div
var divConstraints = document.querySelector('div#constraints');

//recored录制
var recvideo = document.querySelector('video#recplayer');
var btnRecord = document.querySelector('button#record');
var btnPlay = document.querySelector('button#recplay');
var btnDownload = document.querySelector('button#download');

var buffer;
var mediaRecorder;//在全局创建这个变量 相当于let mediaRecorder 然后在开始录制的时候 startRecord初始化=new

function gotMediaStream(stream) {

    videoplay.srcObject = stream;

    var videoTrack = stream.getVideoTracks()[0];//获取多媒体流中的 视频流 所有video 流 [0]只取第一个
    var videoConstraints = videoTrack.getSettings();//拿到video的所有约束 js的对象转换成json格式
    divConstraints.textContent = JSON.stringify(videoConstraints, null, 2);//转换成json格式 第三个参数缩进

    window.stream = stream;//window js全局变量
    //audioplay.srcObject = stream;

    return navigator.mediaDevices.enumerateDevices();
}

function handleError(err) {
    console.log('getUserMedia error:' + err);
}

function gotDevices(deviceInfos) {
    deviceInfos.forEach(function (deviceinfo) {
        var option = document.createElement('option');
        option.text = deviceinfo.label;
        option.value = deviceinfo.deviceId;
        if (deviceinfo.kind === 'audioinput') {
            audioOutput.appendChild(option);
        } else if (deviceinfo.kind === 'audiooutput') {
            audioOutput.appendChild(option);
        } else if (deviceinfo.kind === 'videoinput') {
            videoSource.appendChild(option);
        }

    })
}

function start() {
    if (!navigator.mediaDevices || !navigator.mediaDevices.getUserMedia) {
        console.log('getDisplayMedia is node supported');
        return;
    }
    else {
        var deviceId = videoSource.value;
        var constrants = {
            video: {
                width: 640,
                height: 480,
                frameRate: 30,
                facingMode: 'enviroment'
            },
            audio: true
        }
        navigator.mediaDevices.getUserMedia(constrants).then(gotMediaStream).then(gotDevices).catch(handleError);

    }
}

start();
videoSource.onchange = start;//按键然后触发函数

filtersSelect.onchange = function () {
    videoplay.className = filtersSelect.value;
}//下拉框发生改变 触发onchange

snapshot.onclick = function () {
    picture.getContext('2d').drawImage(videoplay, 0, 0, picture.width, picture.height);
    picture.className = filtersSelect.value;

}//button按钮点击触发onclick
function handleDataAvailable(e) {//录制处理函数 录制数据之后 触发ondataavailable事件 调用这个函数
    if (e && e.data && e.data.size > 0) {
        buffer.push(e.data);
    }
}
function startRecord() {
    buffer = [];//数组
    var options = {
        mimeType: 'video/webm;codecs=vp8'
    }
    if (!MediaRecorder.isTypeSupported(options.mimeType)) {//MediaRecorder全局js变量 看 mimeType支不支持
        console.error(`${options.mimeType} is not supported!`);
        return;
    }
    try {
        mediaRecorder = new MediaRecorder(window.stream, options);//创建对象 
    } catch (e) {
        console.error(`Faile to create MediaRecorder:`, e);
        return;//创建对象可能出错
    }
    mediaRecorder.ondataavailable = handleDataAvailable;//处理事件 事件处理函数收到录制的数据 存储
    mediaRecorder.start(10);//10s一个数据buffer 时间片
}
function stopRecord() {
    mediaRecorder.stop();
}
btnRecord.onclick = () => {
    if (btnRecord.textContent === 'Start Record') {
        btnRecord.textContent = 'Stop Record';//刚开始显示的html是Start Record 录制过程中改变成 Stop Record
        btnDownload.disabled = true;
        btnPlay.disabled = true;
        startRecord();
    } else {
        btnRecord.textContent = 'Start Record';//刚开始显示的html是Start Record 录制过程中改变成 Stop Record
        btnDownload.disabled = false;//这时候可以点击 下载 或者 播放
        btnPlay.disabled = false;
        stopRecord();
    }
}
btnPlay.onclick = () => {
    var blob = new Blob(buffer, { type: 'video/webm' });//第一个参数是存储的数据 第二个参数是类型
    recvideo.src = window.URL.createObjectURL(blob);//video的播放
    recvideo.srcObject = null;//获取直播流 数据 此时不需要这个
    recvideo.controls = true;//播放按钮 暂停按钮
    recvideo.play();

}//开始播放录制的视频

btnDownload.onclick = () => {
    var blob = new Blob(buffer,{ type: 'video/webm' });
    var url = window.URL.createObjectURL(blob);//创建一个URL
    var a = document.createElement('a');//a 标签 创建超链接 通过这个URL下载媒体文件

    a.href = url;
    a.style.display = 'none';//不显示
    a.download = 'aaa.webm';//下载的名字
    a.click();
}