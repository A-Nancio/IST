


function Mobile(scene){
    this.matrix = new THREE.Matrix4();
    this.matrix.makeTranslation(0, -7.5, 0);
    this.supportPole = new Cylinder(0.5, 20, 0.5, this.matrix, scene);
    
    this.rotate = function() {
        
    }

}


function Cylinder(scaleX, scaleY, scaleZ, applyMatrix, scene){
    this.matrix = applyMatrix;
    console.log(this.matrix.elements);

    scalingMatrix = new THREE.Matrix4();
    scalingMatrix.makeScale(scaleX, scaleY, scaleZ);
    this.matrix.multiply(scalingMatrix);
    
    geometry = new THREE.CylinderGeometry(1, 1, 1, 15);
    geometry.applyMatrix4(this.matrix);
    material = new THREE.MeshBasicMaterial( {color: 0x00ff00, wireframe: true});
    this.mesh = new THREE.Mesh(geometry, material);
    scene.add(this.mesh);
}

function Cube(scaleX, scaleY, scaleZ, applyMatrix, scene){
    this.matrix = applyMatrix;
    console.log(this.matrix.elements);

    scalingMatrix = new THREE.Matrix4();
    scalingMatrix.makeScale(scaleX, scaleY, scaleZ);
    this.matrix.multiply(scalingMatrix);
    
    geometry = new THREE.BoxGeometry(1, 1, 1);
    geometry.applyMatrix4(this.matrix);
    material = new THREE.MeshBasicMaterial( {color: 0x00ff00, wireframe: true});
    this.mesh = new THREE.Mesh(geometry, material);
    scene.add(this.mesh);
}

function FirstLayer(){

}



