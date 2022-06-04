var key1isDown, key2isDown, key3isDown, key4isDown, key5isDown, key6isDown, key7isDown, key8isDown, key9isDown;
var arrowLeftisDown, arrowRightisDown;
var spaceKeyisDown, keyQisDown;
var activeCamera, activeBall, cameraFront, cameraPers, cameraFollow;
var scene;
var renderer;
var pool;
var activeCue;
var clock, deltaTime;

const aspectRatio = window.innerWidth/window.innerHeight;
const viewSize = 150;
function createCameras() {

    cameraPers = new THREE.PerspectiveCamera(70,
                                            window.innerWidth / window.innerHeight,
                                            1,
                                            1000);
    cameraPers.position.set(60, 50, 45);
    cameraPers.lookAt(scene.position);

    cameraTop = new THREE.OrthographicCamera(-aspectRatio*viewSize / 2, aspectRatio*viewSize / 2,
                                      viewSize / 2, -viewSize / 2);
    cameraTop.position.set(0, 50, 0);
    cameraTop.up.set(0, 0, -1);
    cameraTop.lookAt(0, 0, 0);

    cameraFollow = new THREE.PerspectiveCamera(70,
                                            window.innerWidth / window.innerHeight,
                                            1,
                                            1000);

    activeCamera = cameraTop;
}
function createScene(){;

    scene = new THREE.Scene();
    scene.add(new THREE.AxesHelper());
    pool = new Pool(scene);
    activeBall = pool.balls[0].base;
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
      case 81: //q
        keyQisDown = true;
        break;
      default:
  }
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
}

const animate = () => {
  deltaTime = clock.getDelta();
  if (arrowLeftisDown) {pool.activeClub.rotate(1);}
  if (arrowRightisDown) {pool.activeClub.rotate(-1);}
  if (spaceKeyisDown) {
    activeBall = pool.shootBall();
    spaceKeyisDown = false;
  }
  if (key1isDown) {activeCamera = cameraTop;}
  if (key2isDown) {activeCamera = cameraPers;}
  if (key3isDown) {activeCamera = cameraFollow;}
  if (key4isDown) {pool.selectCue(0);}
  if (key5isDown) {pool.selectCue(1);}
  if (key6isDown) {pool.selectCue(2);}
  if (key7isDown) {pool.selectCue(3);}
  if (key8isDown) {pool.selectCue(4);}
  if (key9isDown) {pool.selectCue(5);}
  if (keyQisDown) {pool.changeWireFrame(); keyQisDown = false}

  cameraFollow.position.x = activeBall.position.x;
  cameraFollow.position.y = activeBall.position.y + 10;
  cameraFollow.position.z = activeBall.position.z + 10;
  cameraFollow.lookAt(activeBall.position);

  pool.checkCollisions();
  pool.handleCollisions(deltaTime);
  render();
  requestAnimationFrame( animate );

}

function render(){
    renderer.render(scene, activeCamera);
}
