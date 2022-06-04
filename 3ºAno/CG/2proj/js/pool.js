const ballNumber = 16;
const ballRadius = 2;
const ballMass = 1;
const clubLength = 30;
const clubWidth = 0.60;
const wallHeight = 10;
const wallThickness = 1;
const holeRadius = ballRadius*1.5;
const holeHeight = 5;
const tableWidth = 100;
const tableLength = 50;
const wallMass = Infinity;
const frictionScalar = 0.99
const friction = new THREE.Vector3(frictionScalar, -0.98, frictionScalar);

wallMaterial = new THREE.MeshBasicMaterial( {color: 0xb4b8bf, wireframe: false} );
holeMaterial = new THREE.MeshBasicMaterial( {color: 0x242323, wireframe: false} );
tableMaterial = new THREE.MeshBasicMaterial( {color: 0x123456, wireframe: false} );
redBallMaterial = new THREE.MeshBasicMaterial( {color: 0xaf4f60, wireframe: false} );
whiteBallMaterial = new THREE.MeshBasicMaterial( {color: 0xffffff, wireframe: false})
clubGeometry = new THREE.CylinderGeometry(clubWidth, clubWidth, clubLength);
clubMaterial = new THREE.MeshBasicMaterial( {color: 0x704701, wireframe: false} );
activeClubMaterial = new THREE.MeshBasicMaterial( {color: 0xd8eb34, wireframe: false} );

class Pool{
  constructor(scene){
    this.base = new THREE.Object3D();
    scene.add(this.base);

    //create pool table
    this.table = new Table(this.base, tableWidth, tableLength, wallHeight);

    //create clubs
    this.clubs = []
    this.clubs.push(new Club(this.base, tableWidth / 4, tableLength / 2 - 1.5*ballRadius, wallHeight, Math.PI / 2));
    this.clubs.push(new Club(this.base, -tableWidth / 4, tableLength / 2 - 1.5*ballRadius, wallHeight, Math.PI / 2));
    this.clubs.push(new Club(this.base, tableWidth / 4, -tableLength / 2 + 1.5*ballRadius, wallHeight, -Math.PI / 2));
    this.clubs.push(new Club(this.base, -tableWidth / 4, -tableLength / 2 + 1.5*ballRadius, wallHeight, -Math.PI / 2));
    this.clubs.push(new Club(this.base, tableWidth / 2 - 1.5*ballRadius, 0, wallHeight, Math.PI));
    this.clubs.push(new Club(this.base, -tableWidth / 2 + 1.5*ballRadius, 0, wallHeight, 0));
    this.activeClub = this.clubs[0];
    this.activeClub.mesh.material = activeClubMaterial;

    //create balls
    this.balls = []
    this.grid = new Grid(this.base, tableWidth, tableLength, this.balls);
  }

  selectCue(index){
    this.activeClub.mesh.material = clubMaterial;
    this.activeClub = this.clubs[index];
    this.activeClub.mesh.material = activeClubMaterial;
  }

  translate(direction){
    if(direction === 1)
      this.base.translateX(-0.2);
    else if (direction === 2)
      this.base.translateZ(-0.2);
    else if (direction === 3)
      this.base.translateX(0.2);
    else if (direction === 4)
      this.base.translateZ(0.2);
  }

  changeWireFrame(){
    wallMaterial.wireframe = !wallMaterial.wireframe;
    tableMaterial.wireframe = !tableMaterial.wireframe;
    clubMaterial.wireframe = !clubMaterial.wireframe;
    redBallMaterial.wireframe = !redBallMaterial.wireframe
    whiteBallMaterial.wireframe = !whiteBallMaterial.wireframe;
    activeClubMaterial.wireframe = !activeClubMaterial.wireframe;
    holeMaterial.wireframe = !holeMaterial.wireframe;
  }

  shootBall(){
    var newBall = new Ball(this.base, ballRadius, whiteBallMaterial);
    var shootPosition = this.activeClub.aimPoint.getWorldPosition();
    newBall.base.translateOnAxis(shootPosition, 1);
    newBall.base.position.y = ballRadius;
    newBall.velocity.add(this.activeClub.aimVector);
    this.balls.push(newBall);
    return newBall.base;
  }

  checkCollisions(){
    var i, j, w;
    this.colisions = [];

    for(i = 0; i < this.balls.length; i++){

      //ball to wall colision check
      for(w = 0; w < 6; w++)
        if(this.balls[i].checkCollision(this.table.getHole(w)))
          this.colisions.push([this.balls[i], this.table.getHole(w)])

      //ball to hole colision check
      for(w = 0; w < 4; w++)
        if(this.balls[i].checkCollision(this.table.getWall(w)))
          this.colisions.push([this.balls[i], this.table.getWall(w)])

      //ball to ball colision check
      for(j = i + 1; j < this.balls.length; j++)
        if(this.balls[i].checkCollision(this.balls[j]))
          this.colisions.push([this.balls[i], this.balls[j]])
    }
  }

  handleCollisions(deltaTime){
    var i;
    for(i = 0; i < this.colisions.length; i++)
      this.colisions[i][0].handleCollision(this.colisions[i][1], this.base.position);

    for (i = 0; i < this.balls.length; i++)
      this.balls[i].move(deltaTime);
  }
}


class CollidableObject{
  constructor(parent, mass){
    this.velocity = new THREE.Vector3(0, 0, 0);
    this.mass = mass;
    this.base = new THREE.Object3D();
    parent.add(this.base);
  }

  checkCollision(obj){
    if (obj instanceof Ball){ //check for ball to ball collision
      var a = new THREE.Vector3().copy(this.base.position);
      return a.sub(obj.base.position).length() < (this.radius + obj.radius)
    }

    if (obj instanceof Wall){ //check for a ball to wall collision
      var a = new THREE.Box3().setFromObject(this.base);
      var b = new THREE.Box3().setFromObject(obj.base);
      return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
              (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
              (a.min.z <= b.max.z && a.max.z >= b.min.z);
    }

    if (obj instanceof Hole){
      var a = new THREE.Vector3().copy(this.base.position);
      a.sub(obj.base.position);
      return a.length() < this.radius+obj.height/2;
    }
  }

  handleCollision(obj, table_position){
    if (obj instanceof Wall){
      // formula R = D - 2(N.D)N
      var normal = new THREE.Vector3().copy(obj.base.position)
      normal.sub(table_position).normalize() //normal relative to the wall
      var aux = new THREE.Vector3(2,2,2);

      var dot_product = normal.dot(this.velocity)
      var dot_vector = new THREE.Vector3(dot_product, dot_product, dot_product).multiply(normal).multiply(aux);
      this.velocity.sub(dot_vector);
    }

    if (obj instanceof Ball){
      //https://en.wikipedia.org/wiki/Elastic_collision

      var massCalculation = function(m1, m2){
        var x = (2*m2)/(m1+m2);
        return new THREE.Vector3(x,x,x);
      }
      var velocityCalculation = function(v1, v2, x1, x2){
        var v = new THREE.Vector3().copy(v1).sub(v2);
        var x = new THREE.Vector3().copy(x1).sub(x2);

        var a = x.length()*x.length();
        var aux = new THREE.Vector3(a,a,a);

        a = v.dot(x);
        var aux_2 = new THREE.Vector3(a,a,a);
        return aux_2.divide(aux).multiply(x);
      }

      var b1 = new THREE.Vector3().copy(this.velocity); //cache velocity

      var x = massCalculation(this.mass, obj.mass).multiply(velocityCalculation(this.velocity, obj.velocity, this.base.position, obj.base.position));
      this.velocity.sub(x);

      x = massCalculation(obj.mass, this.mass).multiply(velocityCalculation(obj.velocity, b1, this.base.position, obj.base.position));
      obj.velocity.sub(x);
    }

    if (obj instanceof Hole){
      this.velocity = new THREE.Vector3(0,-10,0);
    }
    if (this.checkCollision(obj)){
      this.move();
      if (obj instanceof Ball)
        obj.move();

    }




  }

  rotateY(ang){
    this.base.rotateY(ang);
  }
  rotateZ(ang){24
    this.base.rotateZ(ang);
  }
  rotateX(ang){
    this.base.rotateX(ang);
  }
  translateX(tra){
    this.base.translateX(tra);
  }
  translateY(tra){
    this.base.translateY(tra);
  }
  translateZ(tra){
    this.base.translateZ(tra);
  }
}

class Ball extends CollidableObject{
  constructor(parent, radius, ballMaterial){
    super(parent, ballMass);
    this.radius = radius;
    const segments = 15;

    var geometry = new THREE.SphereGeometry(this.radius, segments, segments);
    var material = ballMaterial;
    this.sphere = new THREE.Mesh( geometry, material );
    this.base.add(this.sphere);
  }

  move(){

    if (this.velocity.y < 0){
      this.base.translateOnAxis(this.velocity, deltaTime);
      return;
    }
    var rotationAxis = new THREE.Vector3(0,1,0).cross(this.velocity).normalize();
    var velocityMag = this.velocity.length();
    var rotationAmount = velocityMag * (Math.PI * 2) / (Math.PI * this.radius * 2);
    this.sphere.rotateOnWorldAxis(rotationAxis, rotationAmount*0.0174532925)
    this.base.translateOnAxis(this.velocity, deltaTime);
    this.velocity.multiply(friction);
  }

  test(){
    var a = new THREE.Vector3().copy(this.velocity);
    var b = new THREE.Vector3(-1,0,-1);
    a.multiply(b);
    this.base.translateOnAxis(a, deltaTime);

  }
}

class Club{
  constructor(base, horizontalPosition, verticalPosition, height, rotation){
    this.aimPoint = new THREE.Object3D();
    this.aimVector = new THREE.Vector3(80, 0, 0);
    this.aimVector.applyAxisAngle(this.aimPoint.up, rotation);
    this.angle = 0;

    this.aimPoint.translateY(height/2);
    this.aimPoint.translateX(horizontalPosition);
    this.aimPoint.translateZ(verticalPosition);
    this.aimPoint.rotateY(rotation);

    this.mesh = new THREE.Mesh(clubGeometry, clubMaterial);
    this.mesh.translateX(-(clubLength/2 + 2.5*ballRadius));
    this.mesh.rotateZ(Math.PI / 2);
    var helper = new THREE.AxesHelper(5);
    this.aimPoint.add(helper);
    this.aimPoint.add(this.mesh);
    base.add(this.aimPoint);
  }

  rotate(orientation){  // 1 = positive rotation, -1 = negative rotation
    if (-Math.PI / 3 < this.angle && orientation == -1){
      this.aimPoint.rotation.y -= .02;
      this.angle -= .02;
      this.aimVector.applyAxisAngle(this.aimPoint.up, -.02);
    }
    if (Math.PI / 3 > this.angle && orientation == 1){
      this.aimPoint.rotation.y += .02;
      this.angle += .02;
      this.aimVector.applyAxisAngle(this.aimPoint.up, .02);
    }
  }
}

class Wall extends CollidableObject{
  constructor(parent, width, height, length){
    super(parent, wallMass);
    this.width = width;
    this.height = height;
    this.length = length;
    var geometry = new THREE.BoxGeometry(width, height, length);
    var mesh = new THREE.Mesh( geometry, wallMaterial );
    this.base.add(mesh);
  }
}

class Grid{
  constructor(parent, tableWidth, tableLength, balls){
    this.base = new THREE.Object3D();
    //adicionar tableWidth-holeRadius e tableLength-holeRadius
    var geometry = new THREE.PlaneGeometry(tableWidth-(holeRadius*5), tableLength-(holeRadius*5), ballNumber/2, ballNumber/2)
    var material = new THREE.MeshBasicMaterial( {color: 0x000000} );
    var plane = new THREE.Mesh(geometry, material);
    parent.add(this.base);
    this.base.add(plane);
    plane.rotateX( - Math.PI / 2);

    var i, ball, coords;
    for (i = 0; i < ballNumber; i++) {
      ball = new Ball(this.base, ballRadius, redBallMaterial);
      coords = geometry.vertices[Math.floor(Math.random() * geometry.vertices.length)]
      ball.base.position.x = coords.x;
      ball.base.position.z = coords.y;
      ball.base.position.y = ballRadius;
      balls.push(ball);

      var a = new THREE.Vector3((Math.random()-0.5)*2, 0, (Math.random()-0.5)*2);
      ball.velocity = a;
    }
  }
}

class Hole extends CollidableObject{
  constructor(parent, radius, height){
    super(parent, Infinity);
    this.radius = radius;
    this.height = height;
    var geometry = new THREE.CylinderGeometry(radius, radius, height, 30);
    var mesh = new THREE.Mesh( geometry, holeMaterial );
    this.base.add(mesh);
  }
}

class Table{
  constructor(parent, tableWidth, tableLength, wallHeight){
    this.base = new THREE.Object3D();
    parent.add(this.base);

    //draw the top
    var geometry = new THREE.BoxGeometry(tableWidth, 1, tableLength);
    var table = new THREE.Mesh( geometry, tableMaterial );
    this.base.add(table);

    //add walls (collidableObjects)
    this.walls = [];
    var wall = new Wall(this.base, wallThickness , wallHeight, tableLength);
    wall.translateX(tableWidth/2 + wallThickness/2);
    this.walls.push(wall);

    wall = new Wall(this.base, wallThickness , wallHeight, tableLength);
    wall.translateX(-(tableWidth/2 + wallThickness/2));
    this.walls.push(wall);

    wall = new Wall(this.base, tableWidth , wallHeight, wallThickness);
    wall.translateZ(-(tableLength/2 + wallThickness/2));
    this.walls.push(wall);

    wall = new Wall(this.base, tableWidth , wallHeight, wallThickness);
    wall.translateZ(tableLength/2 + wallThickness/2);
    this.walls.push(wall);

    //add holes
    this.holes = [];
    var offset = 1;
    var hole = new Hole(this.base, holeRadius, holeHeight);
    hole.translateY(-holeHeight/2 + offset);
    hole.translateZ(tableLength/2 - holeRadius);
    this.holes.push(hole);

    hole = new Hole(this.base, holeRadius, holeHeight);
    hole.translateY(-holeHeight/2 + offset);
    hole.translateZ(-tableLength/2 + holeRadius);
    this.holes.push(hole);

    hole = new Hole(this.base, holeRadius, holeHeight);
    hole.translateY(-holeHeight/2 + offset);
    hole.translateZ(-tableLength/2 + holeRadius);
    hole.translateX(-tableLength + holeRadius);
    this.holes.push(hole);

    hole = new Hole(this.base, holeRadius, holeHeight);
    hole.translateY(-holeHeight/2 + offset);
    hole.translateZ(tableLength/2 - holeRadius);
    hole.translateX(-tableLength + holeRadius);
    this.holes.push(hole);

    hole = new Hole(this.base, holeRadius, holeHeight);
    hole.translateY(-holeHeight/2 + offset);
    hole.translateZ(tableLength/2 - holeRadius);
    hole.translateX(tableLength - holeRadius);
    this.holes.push(hole);

    hole = new Hole(this.base, holeRadius, holeHeight);
    hole.translateY(-holeHeight/2 + offset);
    hole.translateZ(-tableLength/2 + holeRadius);
    hole.translateX(tableLength - holeRadius);
    this.holes.push(hole);

  }
  getTable(){
    return this.geometry;
  }
  getWall(i){
    return this.walls[i];
  }

  getHole(i){
    return this.holes[i];
  }

}
