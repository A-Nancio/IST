const polewidth = 0.75;

class Layer {
  constructor(position, length, height) { //position on X axis relative to the parent layer
    const numSegments = 10; // more segments -> rounder poles
    
    // base makes the connection to the previous layer
    this.primitives = [];
    this.base = new THREE.Object3D();
    this.base.translateX(position);
    this.base.translateY(-height / 2);

    // create primitive for the base
    var poleGeometry = new THREE.CylinderGeometry(polewidth, polewidth, height, numSegments);
    var poleMaterial = new THREE.MeshBasicMaterial({color: 0xC0C0C0, wireframe: true});
    if (height != 0) {  //if height == 0, create just a horizontal pole
      var poleMesh = new THREE.Mesh(poleGeometry, poleMaterial);
      this.base.add(poleMesh);
      this.primitives.push(poleMesh);
    }
    // supports contains all the 'leafs' of that layer
    this.support = new THREE.Object3D();
    this.support.translateY(-height / 2);
    this.base.add(this.support);
    poleGeometry = new THREE.CylinderGeometry(polewidth, polewidth, length, numSegments);
    poleMesh = new THREE.Mesh(poleGeometry, poleMaterial.clone());
    poleMesh.rotateZ(Math.PI / 2);
    this.support.add(poleMesh);
  }

  addToScene(scene){
    scene.add(this.base);
  }

  addLeaf(leaf){
    this.base.add(leaf.base);
  }
}

class Leaf {
  constructor(parentLayer, position, height) { // more Segments --> more round
    const numSegments = 10;
    this.base = new THREE.Object3D()  //  base to attach to support layers
    this.base.translateX(position);
    this.base.translateY(-height / 2);
    parentLayer.add(this.base);

    // create primitive for the base
    if (height != 0) {  //if height is 0, do NOT creat a new mesh
      let poleGeometry = new THREE.CylinderGeometry(polewidth, polewidth, height, numSegments);
      let poleMaterial = new THREE.MeshBasicMaterial({color: 0xC0C0C0, wireframe: true});
      let poleMesh = new THREE.Mesh(poleGeometry, poleMaterial);
      this.base.add(poleMesh);
    }
  }

  addComponent(position, width, height, numSegments, color) {  
    var geometry = new THREE.CylinderGeometry(width, width, height, numSegments);
    var material = new THREE.MeshBasicMaterial({color: color, wireframe: true});
    var primitive = new THREE.Mesh(geometry, material);
    primitive.translateY(-position); //position the component along the leaf base
    this.base.add(primitive);
    return primitive; //in case the user wants to add special transformations to it
  }
}

class Mobile {
  constructor(scene) {

    this.origin = new THREE.Object3D(); //root of the mobile
    scene.add(this.origin);
    //create level 1
    this.level1 = new Layer(0, 60, 15);
    this.level1.addToScene(this.origin);
    
    //level 1 objects
    let leaf = new Leaf(this.level1.support, -15, 10);
    leaf.addComponent(5, 4, 3, 4, 0xFF0000).rotateX(Math.PI / 2);
    
    leaf = new Leaf(this.level1.support, -29, 25);
    leaf.addComponent(-2, 6, 3, 4, 0x3883fc).rotateX(Math.PI / 2);
    leaf.addComponent(-2, 4, 4, 4, 0xFF0000).rotateX(Math.PI / 2);
    leaf.addComponent(12.5, 4, 3, 4, 0x3883fc).rotateX(Math.PI / 2);
    
    leaf = new Leaf(this.level1.support, 15, 5);
    leaf.addComponent(7, 5, 3, 20, 0xff7300).rotateX(Math.PI / 2);
    leaf.addComponent(7, 3, 4, 20, 0x3883fc).rotateX(Math.PI / 2);
    
    leaf = new Leaf(this.level1.support, 29, 12.5);
    leaf.addComponent(6.25, 3, 3, 3, 0x54ff6e).rotateX(Math.PI / 2);
    
    //create level 2
    this.level2 = new Layer(0, 40, 20); //bar 1
    this.level2.addToScene(this.level1.support);
    
    var level2_1 = new Layer(0, 40, 0); //bar 2
    level2_1.support.rotateY(Math.PI / 2);
    level2_1.addToScene(this.level2.support);
    
    //level 2 objects
    leaf = new Leaf(this.level2.support, 19.5, 15);
    leaf.addComponent(7.5, 4, 15, 4, 0xff0400);
    leaf.addComponent(7.5, 4.5, 9, 4, 0x3883fc);
    
    leaf = new Leaf(this.level2.support, -19.5, 15);
    leaf.addComponent(7.5, 4, 15, 20, 0xe8cd00);
    leaf.addComponent(7.5, 4.5, 9, 20, 0x3883fc);
    
    leaf = new Leaf(level2_1.support, 19.5, 15);
    leaf.addComponent(0, 6, 3, 20, 0x54ff6e);
    leaf.addComponent(7.5, 3, 3, 20, 0xff7300);

    leaf = new Leaf(level2_1.support, -19.5, 15);
    leaf.addComponent(0, 6, 3, 4, 0xff0400);
    leaf.addComponent(7.5, 3, 3, 4, 0xe8cd00);

    //create level 3
    this.level3 = new Layer(0, 30, 30);
    this.level3.addToScene(this.level2.support);
    
    //level 3 objects

    leaf = new Leaf(this.level3.support, 14.5, 0);
    leaf.addComponent(0, 4, 2, 20, 0xe8cd00).rotateZ(Math.PI / 2);

    leaf = new Leaf(this.level3.support, -14.5, 0);
    leaf.addComponent(0, 4, 2, 20, 0xff7300).rotateZ(Math.PI / 2);

    leaf = new Leaf(this.level3.support, 0, 10);
    leaf.addComponent(5, 4, 2, 4, 0xffffff).rotateX(Math.PI / 2);
    }

    rotate(layer, direction){
      if(direction) {//left
        layer.base.rotation.y += .02;
      } else {
        layer.base.rotation.y -= .02;
      }
    }

  translate(direction){
    if(direction === 1)
      this.origin.translateX(-0.2);
    else if (direction === 2)
      this.origin.translateZ(-0.2);
    else if (direction === 3)
      this.origin.translateX(0.2);
    else if (direction === 4) 
      this.origin.translateZ(0.2);
  }
  
  changeWireFrame(){
    this.level1.base.traverse(function(node){
      if (node instanceof THREE.Mesh)
        node.material.wireframe = !node.material.wireframe;
    });
  }
}

