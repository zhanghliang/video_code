'use strict'

var localVideo = document.querySelector('video#localvideo');
var remoteVideo = document.querySelector('video#remotevideo');

var btnStart = document.querySelector('button#start');
var btnCall = document.querySelector('button#call');
var btnhangup = document.querySelector('button#hangup');

var localStream;
var pc1, pc2;

var offer = document.querySelector('textarea#offer');
var answer = document.querySelector('textarea#answer');

function handleError(err) {
    console.error('Failed to get Media Stream!', err);
}

function getMediaStream(stream) {
    localVideo.srcObject = stream;
    localStream = stream;
}
function start() {
    if (!navigator.mediaDevices || !navigator.mediaDevices.getUserMedia) {
        console.error('the getUserMedia is not supported!');
        return;
    }
    else {
        var constraints = {
            video: true,
            audio: false
        };
        navigator.mediaDevices.getUserMedia(constraints).then(getMediaStream).catch(handleError);
    }
}
function getRemoteStream(e) {
    remoteVideo.srcObject = e.streams[0];//会有多个流 只取第一个 这样可以显示源端的视频
}

function handleOfferErr(err) {
    console.error('Failed to create Offer:', err);
}
function handleAnswerError(err) {
    console.error('Failed to create Answer:', err);
}
function getOffer(desc) {
    pc1.setLocalDescription(desc);//调用完会触发onicecandidate事件所以下面先绑定一下
    offer.value = desc.sdp;
    //send desc to signal 将这条信息发送给信令服务器
    //receive desc from signal
    pc2.setRemoteDescription(desc);
    pc2.createAnswer().then(getAnswer).catch(handleAnswerError);

}
function getAnswer(desc) {
    pc2.setLocalDescription(desc);
    answer.value = desc.sdp;
    //send desc to signal 将这条信息发送给信令服务器
    //receive desc from signal
    pc1.setRemoteDescription(desc);
    //协商完成
}
function call() {
    pc1 = new RTCPeerConnection();//本机自己候选类型 调用者

    //候选者
    pc1.onicecandidate = (e) => {
        pc2.addIceCandidate(e.candidate);//将对端的 候选者信息放入自己里面进行比较 检测通路
    }//这里是收到STURN TURN发来的候选者 所有通路 排序 因为都是本地 相当于没有信令服务器 所以是pc1 pc2 如果是两个客户端这里直接一个pc即可

    pc2 = new RTCPeerConnection();

    pc2.onicecandidate = (e) => {
        pc1.addIceCandidate(e.candidate);
    }//收到来自STURN TURN发来的候选者信息 然后直接发送给对端 让对端放入自己
    //addIceCandidate将对方的通路放入自己里面进行检测 找到通路

    pc2.ontrack = getRemoteStream;//pc2被调用方 数据来的时候触发ontrack事件

    //获取本机的媒体数据
    localStream.getTracks().forEach((track) => {
        pc1.addTrack(track, localStream);//CreatePeerConnect 获取媒体数据
    });
    //进行媒体协商
    var offerOptions = {
        offerToRecieveAudio: 0,
        offerToRecieveVideo: 1
    }
    pc1.createOffer(offerOptions).then(getOffer).catch(handleOfferErr);
}
function hangup(){
    pc1.close();
    pc2.close();
    pc1 = null;
    pc2 = null;
}
btnStart.onclick = start;
btnCall.onclick = call;
btnhangup.onclick = hangup;