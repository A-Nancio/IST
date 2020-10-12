

var camera;
var scene;
var renderer;

function createScene(){;

    scene = new THREE.Scene();
    camera = new THREE.PerspectiveCamera(70,
                                        window.innerWidth/window.innerHeight,
                                        1,
                                        1000);
    camera.position.z = 30;
    
    var mobile = new Mobile(scene);
}

function init(){

    renderer = new THREE.WebGLRenderer();
    renderer.setSize(window.innerWidth, window.innerHeight);
    document.body.appendChild(renderer.domElement);

    createScene();

    render();
}

function animate(){
    mobile.rotate();
    
    //scene.rotation.y += .02;
    
    render();

    requestAnimationFrame( animate );
}

function render(){
    renderer.render(scene, camera);
}