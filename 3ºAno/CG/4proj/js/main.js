
var keyRisDown, keySisDown, keyIisDown, keyWisDown, keyPisDown, keyDisDown, keyBisUp = true;
var pauseCamera, cameraPers;
var scene, pauseScene;
var renderer;
var clock, deltaTime, pause = false;
var controls;

var aspectRatio = window.innerWidth/window.innerHeight;
const viewSize = 20;
function createCameras() {

    cameraPers = new THREE.PerspectiveCamera(70,
      window.innerWidth / window.innerHeight,
      1,
      600);
      cameraPers.position.set(5, 5, 20);
      cameraPers.lookAt(0,0,0); 
      controls = new THREE.OrbitControls(cameraPers, renderer.domElement);
      controls.update();

    pauseCamera = new THREE.OrthographicCamera(-aspectRatio*viewSize / 2, aspectRatio*viewSize / 2,
                                                viewSize / 2, -viewSize / 2);
    pauseCamera.position.set(10,0,0);
    pauseCamera.lookAt(0,0,0);
}
function createScene(){;
    scene = new THREE.Scene();
    pauseScene = new THREE.Scene();
    create_golf(scene);
    create_pause_message(pauseScene);
}
function onKeyDown(e) {
    'use strict';

    switch (e.keyCode) {
      case 66:  //B(b)
        keyBisUp = false;
        break;
      case 68:  //D(d)
        keyDisDown = true;
        break;
      case 73:  //I(i)
        keyIisDown = true;
        break;
      case 80:  //P(p)
        keyPisDown = true;
        break;
      case 82:  //R(r)
        keyRisDown = true;
        break;
      case 83:  //S(s)
        keySisDown = true;
        break;
      case 87:  //W(w)
        keyWisDown = true;
        break;
      default:
    }
}

function onKeyUp(e) {
    switch (e.keyCode) {
      case 66:  //B(b)
        keyBisUp = true;
        break;
      case 68:  //D(d)
        keyDisDown = false;
        break;
      case 73:  //I(i)
        keyIisDown = false;
        break;
      case 80:  //P(p)
        keyPisDown = false;
        break;
      case 82:  //R(r)
        keyRisDown = false;
        break;
      case 83:  //S(S)
        keySisDown = false;
        break;
      case 87:  //W(w)
        keyWisDown = false;
        break;
      default:
  }
}

function onWindowResize(e){
  aspectRatio = window.innerWidth/window.innerHeight;
  cameraPers.aspect = aspectRatio;
  cameraPers.updateProjectionMatrix();
  renderer.setSize(window.innerWidth, window.innerHeight);
}

function init(){
  renderer = new THREE.WebGLRenderer({alpha: true});
  renderer.autoClear = false;
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
  deltaTime = clock.getDelta(deltaTime);
  if (keySisDown) {pause = !pause; keySisDown = false;}
  if (keyBisUp) {moveBall(deltaTime, pause);}
  if (keyDisDown) {toggleDirectionalLight(); keyDisDown = false;}
  if (keyPisDown) {togglePointLight(); keyPisDown = false;}
  if (keyWisDown) {toggleWireFrame(); keyWisDown = false;}
  if (keyIisDown) {toggleLightCalculation(); keyIisDown = false;}
  if (keyRisDown && pause) {  //reset can only be pressed if the game is paused
    cameraPers.position.set(5, 5, 20);
    cameraPers.lookAt(0, 0, 0);
    resetScene(); keyRisDown = false;
  }
  
  rotatePole(deltaTime, pause);
  controls.update();
  render();
  requestAnimationFrame( animate );

}

function render(){
    renderer.clear();
    renderer.render(scene, cameraPers); 
    if (pause) 
      renderer.render(pauseScene, pauseCamera);
}
