
var keyQisDown, keyWisDown, keyAisDown, keyDisDown, keyZisDown, keyCisDown;
var arrowUpisDown, arrowLeftisDown, arrowRightisDown, arrowDownisDown;
var activeCamera, cameraFront, cameraTop, cameraLat;
var scene;
var renderer;
var mobile;

const aspectRatio = window.innerWidth/window.innerHeight;
const viewSize = 100;
function createCameras() {

    cameraFront = new THREE.OrthographicCamera(-aspectRatio*viewSize / 2, aspectRatio*viewSize / 2,
                                      viewSize / 2, -viewSize / 2);
    cameraFront.position.set(0, 0, 100);
    cameraFront.lookAt(0, -50, 0);

    cameraTop = new THREE.OrthographicCamera(-aspectRatio*viewSize / 2, aspectRatio*viewSize / 2,
                                      viewSize / 2, -viewSize / 2);
    cameraTop.position.set(0, 30, 0);
    cameraTop.up.set(0, 0, -1);
    cameraTop.lookAt(0, 0, 0);

    cameraLat = new THREE.OrthographicCamera(-aspectRatio*viewSize / 2, aspectRatio*viewSize / 2,
                                      viewSize / 2, -viewSize / 2);
    cameraLat.position.set(0, -45, 100);

    activeCamera = cameraFront;
}
function createScene(){;

    scene = new THREE.Scene();
    scene.add(new THREE.AxesHelper(5));
    mobile = new Mobile(scene);
}
function onKeyDown(e) {
    'use strict';

    switch (e.keyCode) {
      case 49:  //1
        activeCamera = cameraFront;
        break;
      case 50:  //2
        activeCamera = cameraTop;
        break;
      case 51:  //3
        activeCamera = cameraLat;
        break;
      case 52:  //4
        mobile.changeWireFrame();
        break;
      case 81:  //Q
        keyQisDown = true;
        break;
      case 87:  //W
        keyWisDown = true;
        break;
      case 65:  //A
        keyAisDown = true;
        break;
      case 68:  //D
        keyDisDown = true;
        break;
      case 67:  //C
        keyCisDown = true;
        break;
      case 90:  //Z
        keyZisDown = true;
        break;
      case 37: //left
        arrowLeftisDown = true;
        break;
      case 38: //up
        arrowUpisDown = true;
        break;
      case 39: //right
        arrowRightisDown = true;
        break;
      case 40: //down
        arrowDownisDown = true;
        break;
      default:
    }
}

function onKeyUp(e) {
    switch (e.keyCode) {
      case 81:  //Q
        keyQisDown = false;
        break;
      case 87:  //W
        keyWisDown = false;
        break;
      case 65:  //A
        keyAisDown = false;
        break;
      case 68:  //D
        keyDisDown = false;
        break;
      case 67:  //C
        keyCisDown = false;
        break;
      case 90:  //Z
        keyZisDown = false;
        break;
      case 37:  //left
        arrowLeftisDown = false;
        break;
      case 38:  //up
        arrowUpisDown = false;
        break;
      case 39:  //right
        arrowRightisDown = false;
        break;
      case 40:  //down
        arrowDownisDown = false;
        break;
      default:

  }
}
function init(){

    renderer = new THREE.WebGLRenderer();
    renderer.setSize(window.innerWidth, window.innerHeight);
    document.body.appendChild(renderer.domElement);

    createScene();
    createCameras();
    window.addEventListener("keydown", onKeyDown);
    window.addEventListener("keyup", onKeyUp);
    render();
}

const animate = () => {
  if (keyQisDown) {mobile.rotate(mobile.level1, 1);}
    if (keyWisDown) {mobile.rotate(mobile.level1, 0);}
    if (keyAisDown) {mobile.rotate(mobile.level2, 1);}
    if (keyDisDown) {mobile.rotate(mobile.level2, 0);}
    if (keyCisDown) {mobile.rotate(mobile.level3, 0);}
    if (keyZisDown) {mobile.rotate(mobile.level3,1);}
    if (arrowUpisDown) {mobile.translate(2);}
    if (arrowLeftisDown) {mobile.translate(1);}
    if (arrowDownisDown) {mobile.translate(4);}
    if (arrowRightisDown) {mobile.translate(3);}

    render();
    requestAnimationFrame( animate );

}



function render(){
    renderer.render(scene, activeCamera);
}
