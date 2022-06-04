var key1isDown, key2isDown, key3isDown, key4isDown, key5isDown, key6isDown, key7isDown, key8isDown, key9isDown;
var arrowLeftisDown, arrowRightisDown;
var spaceKeyisDown, keyQisDown, keyWisDown, keyEisDown;
var activeCamera, activeBall, cameraFront, cameraPers, cameraFollow;
var scene;
var renderer;
var pool;
var activeCue;
var clock, deltaTime;

var aspectRatio = window.innerWidth/window.innerHeight;
const viewSize = 20;
function createCameras() {

    cameraPers = new THREE.PerspectiveCamera(70,
                                            window.innerWidth / window.innerHeight,
                                            1,
                                            400);
    cameraPers.position.set(5, 5, 20);
    cameraPers.lookAt(0,0,0);

    cameraSide = new THREE.OrthographicCamera(-aspectRatio*viewSize / 2, aspectRatio*viewSize / 2,
                                      viewSize / 2, -viewSize / 2);

    cameraSide.position.set(0, 0, 10);
    cameraSide.lookAt(0, 0, 0);

    cameraTop = new THREE.OrthographicCamera(-aspectRatio*viewSize / 2, aspectRatio*viewSize / 2,
                                      viewSize / 2, -viewSize / 2);
    cameraTop.position.set(0, 10, 0);
    cameraTop.lookAt(0, 0, 0);
    

    activeCamera = cameraSide;
}
function createScene(){;

    scene = new THREE.Scene();
    //scene.add(new THREE.AxesHelper(10));
    stage = new Stage(scene);
}
function onKeyDown(e) {
    'use strict';

    switch (e.keyCode) {
      case 37: //left
        arrowLeftisDown = true;
        break;
      case 39: //right
        arrowRightisDown = true;
        break;
      case 49:  //1
        key1isDown = true;
        break;
      case 50:  //2
        key2isDown = true;
        break;
      case 51:  //3
        key3isDown = true;
        break;
      case 52:  //4
        key4isDown = true;
        break;
      case 53:  //5
        key5isDown = true;
        break;
      case 54: //6
        key6isDown = true;
        break;
      case 55: //7
        key7isDown = true;
        break;
      case 56: //8
        key8isDown = true;
        break;
      case 57: //9
        key9isDown = true;
        break;
      case 69: //E
        keyEisDown = true;
        break;
      case 81: //q
        keyQisDown = true;
        break;
      case 87: //W
        keyWisDown = true;
        break;
      default:
    }
}

function onKeyUp(e) {
    switch (e.keyCode) {
      case 32:  //space
        spaceKeyisDown = true;
        break;
      case 37:  //left
        arrowLeftisDown = false;
        break;
      case 39:  //right
        arrowRightisDown = false;
        break;
      case 49:
        key1isDown = false;
        break;
      case 50:
        key2isDown = false;
        break;
      case 51:
        key3isDown = false;
        break;
      case 52:  //4
        key4isDown = false;
        break;
      case 53:  //5
        key5isDown = false;
        break;
      case 54: //6
        key6isDown = false;
        break;
      case 55: //7
        key7isDown = false;
        break;
      case 56: //8
        key8isDown = false;
        break;
      case 57: //9
        key9isDown = false;
        break;
      case 69: //E
        keyEisDown = false;
      case 81: //q
        keyQisDown = false;
        break;
      case 87: //w
        keyWisDown = false;
        break;
      default:
  }
}

function onWindowResize(e){
  aspectRatio = window.innerWidth/window.innerHeight;

  if (activeCamera instanceof THREE.OrthographicCamera){
    activeCamera.left = -aspectRatio*viewSize / 2;
    activeCamera.right = aspectRatio*viewSize / 2;
    activeCamera.top = viewSize / 2;
    activeCamera.bottom = -viewSize / 2;
  }

  if (activeCamera instanceof THREE.PerspectiveCamera){
    activeCamera.aspect = aspectRatio;
  }

  activeCamera.updateProjectionMatrix();

  renderer.setSize(window.innerWidth, window.innerHeight);
}

function init(){

  renderer = new THREE.WebGLRenderer();
  renderer.setSize(window.innerWidth, window.innerHeight);
  document.body.appendChild(renderer.domElement);
  clock = new THREE.Clock();
  createScene();
  createCameras();
  window.addEventListener("keydown", onKeyDown);
  window.addEventListener("keyup", onKeyUp);
  window.addEventListener("resize", onWindowResize);
}

const animate = () => {
  deltaTime = clock.getDelta();
  if (arrowLeftisDown) stage.rotate(-1);
  if (arrowRightisDown) stage.rotate(1);
  if (key1isDown) {stage.toggleSpotlight(0); key1isDown = false;}
  if (key2isDown) {stage.toggleSpotlight(1); key2isDown = false;}
  if (key3isDown) {stage.toggleSpotlight(2); key3isDown = false;}
  if (key4isDown) {activeCamera = cameraPers; key4isDown = false;} // not needed
  if (key5isDown) {activeCamera = cameraSide; key5isDown = false;}
  if (key6isDown) {activeCamera = cameraTop; key6isDown = false;}
  if (keyWisDown) {stage.toggleLightCalculation(); keyWisDown = false}
  if (keyEisDown) {stage.toggleGouraudPhong(); keyEisDown = false}
  if (keyQisDown) {stage.toggleDirectionalLight(); keyQisDown = false}

  render();
  requestAnimationFrame( animate );

}

function render(){
    renderer.render(scene, activeCamera);
}
