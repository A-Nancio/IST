
//SKYBOX LOADING
var textureLoader = new THREE.TextureLoader();
var skyBoxLoader = new THREE.CubeTextureLoader();
const skyBox = skyBoxLoader.load([
    'textures/skyBoxRight.bmp', 'textures/skyBoxLeft.bmp',
    'textures/skyBoxTop.bmp', 'textures/skyBoxBottom.bmp',
    'textures/skyBoxFront.bmp', 'textures/skyBoxBack.bmp'
]);

//TEXTURE LOADING
pattern = textureLoader.load("textures/checkers.png");
grassBump = textureLoader.load("textures/grass_bump.jpg");
golfBallNormal = textureLoader.load("textures/golfball_normal.jpg");
pauseImage = textureLoader.load("textures/pause_image.png");

pattern.wrapS = pattern.wrapT = grassBump.wrapS = grassBump.wrapT = THREE.RepeatWrapping;
pattern.offset.set(1, 1)
pattern.repeat.set(100,100);

var meshBall, meshFloor, meshPole, meshFlag;
var directionalLight, pointLight;

//BASIC MATERIALS
var lightCalculation = true;
const ballBasicMaterial = new THREE.MeshBasicMaterial({color: 0x239ade, wireframe: false});
const floorBasicMaterial = new THREE.MeshBasicMaterial({color: 0x117017, wireframe: false});
const poleBasicMaterial = new THREE.MeshBasicMaterial({color: 0x969696, wireframe: false});
const flagBasicMaterial = new THREE.MeshBasicMaterial({color: 0xcc0000, wireframe: false});

//MATERIALS WITH SHADING AND TEXTURES
const ballMaterial = new THREE.MeshPhongMaterial({ 
    color: 0x239ade,
    normalMap: golfBallNormal,
    shininess: 100,
    specular: 0x454545,
    shadowSide: THREE.DoubleSide,
});
const floorMaterial = new THREE.MeshPhongMaterial({
    shininess: 9.5,
    color: 0x969696,
    map: pattern,
    bumpMap: grassBump,
    bumpScale: 1,
});
const poleMaterial = new THREE.MeshPhongMaterial({color: 0x969696});
const flagMaterial = new THREE.MeshPhongMaterial({color: 0xcc0000});

//LIGHTS
var directionalLight = new THREE.DirectionalLight(0xffffff, 1);
directionalLight.position.set(20, 30, 0);

var pointLight = new THREE.PointLight(0xffffff, 1.2, 1000);
pointLight.position.set(-20,30, 0);


function create_golf(scene) {
	scene.background = skyBox;
    hole = new THREE.Object3D();
    scene.add(hole);

    scene.add(pointLight);
    scene.add(directionalLight);

    //MESHES
	meshBall = new THREE.Mesh(new THREE.SphereGeometry(0.5,30,30), ballMaterial);
	meshBall.translateY(0.5);
	meshBall.receiveShadow = true;
    meshBall.castShadow = true;
    meshBall.translateX(3);
    hole.add(meshBall);
	
	meshFloor = new THREE.Mesh(new THREE.PlaneGeometry(500,500, 10,10), floorMaterial);
    meshFloor.rotation.x -= Math.PI / 2;
    scene.add(meshFloor);

    meshPole = new THREE.Mesh(new THREE.CylinderGeometry(.2, .2, 15, 20), poleMaterial);
    meshPole.translateY(7.5);
    scene.add(meshPole);

    meshFlag = new THREE.Mesh(new THREE.CylinderGeometry(1, 1, .3, 3), flagMaterial);
    meshFlag.translateY(6.5);
    meshFlag.translateX(-.5);
    meshFlag.rotateX(Math.PI / 2);
    meshFlag.rotateY(-Math.PI /2);
    meshPole.add(meshFlag);    
}  

function resetScene() {
    //reset object positions
    let up = new THREE.Vector3(0,1,0);
    hole.setRotationFromAxisAngle(up, 0);
    meshBall.rotation.x = 0;
    meshPole.setRotationFromAxisAngle(up, 0);

    //reset lights
    directionalLight.visible = true;
    pointLight.visible = true;

    //set all wireframes to false
    ballBasicMaterial.wireframe = false;
    floorBasicMaterial.wireframe = false;
    poleBasicMaterial.wireframe = false;
    flagBasicMaterial.wireframe = false;
    ballMaterial.wireframe = false;
    floorMaterial.wireframe = false;
    poleMaterial.wireframe = false;
    flagMaterial.wireframe = false;

    //set objects to starting materials
    meshBall.material = ballMaterial;
    meshBall.material = ballMaterial;
    meshFloor.material = floorMaterial;
    meshPole.material = poleMaterial;
    meshFlag.material = flagMaterial;
}

function moveBall(deltaTime, pause) {
    if (!pause) {
        hole.rotateY(-deltaTime);
        meshBall.rotateX(deltaTime * (Math.PI * 2) / (Math.PI * 2));
    }
}

function rotatePole(deltaTime, pause) {if (!pause) meshPole.rotateY(deltaTime);}

function toggleDirectionalLight() {directionalLight.visible = !directionalLight.visible;}

function togglePointLight() {pointLight.visible = !pointLight.visible;}

function toggleWireFrame() {
    ballBasicMaterial.wireframe = !ballBasicMaterial.wireframe;
    floorBasicMaterial.wireframe = !floorBasicMaterial.wireframe;
    poleBasicMaterial.wireframe = !poleBasicMaterial.wireframe;
    flagBasicMaterial.wireframe = !flagBasicMaterial.wireframe;
    ballMaterial.wireframe = !ballMaterial.wireframe;
    floorMaterial.wireframe = !floorMaterial.wireframe;
    poleMaterial.wireframe = !poleMaterial.wireframe;
    flagMaterial.wireframe = !flagMaterial.wireframe;
}

function toggleLightCalculation() {
    if (lightCalculation == true) {
        meshBall.material = ballBasicMaterial;
        meshFloor.material = floorBasicMaterial;
        meshPole.material = poleBasicMaterial;
        meshFlag.material = flagBasicMaterial;
        lightCalculation = false;
    }
    else {
        meshBall.material = ballMaterial;
        meshFloor.material = floorMaterial;
        meshPole.material = poleMaterial;
        meshFlag.material = flagMaterial;
        lightCalculation = true;
    }
}

function create_pause_message(scene) {
    scene.add(new THREE.Mesh(
        new THREE.CubeGeometry(2, 5, 5),
        new THREE.MeshBasicMaterial({color: 0x000000, transparent: true, alphaMap: pauseImage})
    ));
}
