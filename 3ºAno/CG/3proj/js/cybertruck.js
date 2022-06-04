const podiumRadius = 9.05;
const wheelRadius = 1.45;
/*
Diametro do palanque: 18.1
Tamanho do chassis: 11.7
Raio da roda: 1.45
Altura do carro: 5.01
*/

wheelMaterialBasic = new THREE.MeshBasicMaterial( {color: 0x00000, wireframe: false} );
wheelMaterialLambert = new THREE.MeshLambertMaterial( {color: 0x00000, wireframe: false} );
wheelMaterialPhong = new THREE.MeshPhongMaterial( {color: 0x00000, specular: 0x112211, shininess: 3, wireframe: false} );
wheelMaterialLambert.side =  THREE.DoubleSide;
wheelMaterialPhong.side = THREE.DoubleSide;



windowMaterialBasic = new THREE.MeshBasicMaterial( {color: 0x00000, wireframe: false});
windowMaterialLambert = new THREE.MeshLambertMaterial( {color: 0xb4b8bf, wireframe: false});
windowMaterialPhong = new THREE.MeshPhongMaterial( {color: 0x001a82, specular: 0x1ff324, shininess: 3, wireframe: false});

chassisMaterialBasic = new THREE.MeshBasicMaterial( {color: 0xc2c2c2, wireframe: false});
chassisMaterialLambert = new THREE.MeshLambertMaterial( {color: 0xc2c2c2, wireframe: false});
chassisMaterialPhong = new THREE.MeshPhongMaterial( {color: 0xc2c2c2, specular: 0xfaa341, shininess: 3, wireframe: false});

whiteMaterialBasic = new THREE.MeshBasicMaterial( {color: 0xffffff, wireframe: false});
whiteMaterialLambert = new THREE.MeshLambertMaterial( {color: 0xffffff, wireframe: false});
whiteMaterialPhong = new THREE.MeshPhongMaterial( {color: 0xffffff, specular: 0x132451, shininess: 3, wireframe: false});
whiteMaterialLambert.side =  THREE.DoubleSide;
whiteMaterialPhong.side = THREE.DoubleSide;


floorMaterialBasic = new THREE.MeshBasicMaterial( {color: 0x12004a, wireframe: false} );
floorMaterialLambert = new THREE.MeshLambertMaterial( {color: 0x12004a, wireframe: false} );
floorMaterialPhong = new THREE.MeshPhongMaterial( { color: 0x12004a, specular: 0x112211, shininess: 5, wireframe: false } );


wheelMaterial = new THREE.MeshBasicMaterial( {color: 0x00000, wireframe: true} );

greyMaterialBasic = new THREE.MeshBasicMaterial( {color: 0x363636, wireframe: false});
greyMaterialLambert = new THREE.MeshLambertMaterial( {color: 0x363636, wireframe: false});
greyMaterialPhong = new THREE.MeshPhongMaterial( {color: 0x363636, specular: 0x132451, shininess: 5, wireframe: false});


function createWheel(positionX, positionZ) {
	let geometry = new THREE.CylinderGeometry(wheelRadius, wheelRadius, 0.9, 30);
	let mesh = new THREE.Mesh(geometry, wheelMaterialBasic);
	mesh.translateX(positionX);
	mesh.translateZ(positionZ);
	mesh.translateY(wheelRadius);
	mesh.rotateX(Math.PI / 2);
	return mesh;
}

function createBrakes(positionX, positionY, positionZ) {
	let brakeGeo = new THREE.CylinderGeometry(0.25, 0.25, 1.6, 5);
	let brakeMesh = new THREE.Mesh(brakeGeo, greyMaterialBasic);
	brakeMesh.translateX(positionX);
	brakeMesh.translateY(positionY);
	brakeMesh.translateZ(positionZ);
	return brakeMesh;
}

function addWheelSupport(positionX) {
	let geometry = new THREE.CylinderGeometry(0.25, 0.25, 5.4, 5);
	let mesh = new THREE.Mesh(geometry, greyMaterialBasic);
	mesh.translateX(positionX);
	mesh.translateY(wheelRadius);
	mesh.rotateX(Math.PI / 2);
	return mesh;
}

function createChassis(base) {
	let geometry = new THREE.BoxGeometry(15.3, 0.5, 6.15);
	let mesh = new THREE.Mesh(geometry, greyMaterialBasic);
	mesh.translateY(2.2);
	mesh.translateX(-1.5);
	base.add(mesh);

	base.add(addWheelSupport(6.25));
	base.add(addWheelSupport(-5.5));

	//BRAKES
	base.add(createBrakes(6.25, 2.3, 1.85));
	base.add(createBrakes(6.25, 2.3, -1.85));
	base.add(createBrakes(-5.5, 2.3, -1.85));
	base.add(createBrakes(-5.5, 2.3, 1.85));

}

function createCybertruck() {
	let base = new THREE.Object3D();
	base.translateY(1);
	base.add(createWheel(6.25, 2.7));
	base.add(createWheel(6.25, -2.7));
	base.add(createWheel(-5.5, 2.7));
	base.add(createWheel(-5.5, -2.7));
	createChassis(base);
	carGeometry = new THREE.Geometry();
	carGeometry.materials = [windowMaterialBasic, chassisMaterialBasic, whiteMaterialBasic, windowMaterialPhong, chassisMaterialPhong, whiteMaterialPhong, windowMaterialLambert, chassisMaterialLambert, whiteMaterialLambert];

	carGeometry.vertices.push(
		new THREE.Vector3(3.45, 5.01, 0), 		//0
		new THREE.Vector3(1.5, 5.57, 1.75), 	//1
		new THREE.Vector3(1.5, 5.57, -1.75), 	//2
		new THREE.Vector3(6, 4.29, -2.35), 		//3
		new THREE.Vector3(6, 4.29, 2.35), 		//4

		new THREE.Vector3(8, 3.72, -2.85),		//5
		new THREE.Vector3(8.9, 3.47, -1.8),		//6
		new THREE.Vector3(8.9, 3.47, 1.8),		//7
		new THREE.Vector3(8.2, 3.72, 2.85),		//8
		new THREE.Vector3(7.65, 3.824, 0),		//9
		new THREE.Vector3(1.2, 5.66, 2.08),		//10
		new THREE.Vector3(1.2, 5.66, -2.08),	//11

		new THREE.Vector3(9, 3.30, 1.8),		//12
		new THREE.Vector3(9, 3.30, -1.8),		//13
		new THREE.Vector3(8.1, 3.52, 2.9),		//14
		new THREE.Vector3(8.1, 3.52, -2.9),		//15

		new THREE.Vector3(7.9, 2.2, 2.8),		//16
		new THREE.Vector3(7.9, 2.2, -2.8),		//17
		new THREE.Vector3(8.8, 2.2, 1.65),		//18
		new THREE.Vector3(8.8, 2.2, -1.65),		//19

		new THREE.Vector3(6.5, 3.95, -2.7),		//20
		new THREE.Vector3(1.2, 5.44, -2.18),	//21
		new THREE.Vector3(-2.7, 5, -2.4),		//22
		new THREE.Vector3(-2.8, 4.55, -2.7),	//23
		new THREE.Vector3(-9, 4.5, -2.85),		//24

		new THREE.Vector3(6.5, 3.95, 2.7),		//25
		new THREE.Vector3(1.2, 5.46, 2.18),		//26
		new THREE.Vector3(-2.7, 5.09, 2.4),		//27
		new THREE.Vector3(-2.8, 4.55, 2.7),		//28
		new THREE.Vector3(-9, 4.5, 2.85),		//29

		new THREE.Vector3(-9,	4.2, -2.85),	//30
  		new THREE.Vector3(-9,	4.2, 2.85),		//31
		new THREE.Vector3(-9, 2.2, -2.7),		//32
		new THREE.Vector3(-9, 2.2, 2.7),		//33
		new THREE.Vector3(-9, 3.4, 0),			//34
 		new THREE.Vector3(-3.2, 5.35, 0),		//35


		//separacao vidros
	)

	carGeometry.faces.push(
		//front window
		new THREE.Face3(0, 2, 1),
		new THREE.Face3(0, 1, 4),
		new THREE.Face3(0, 4, 3),
		new THREE.Face3(0, 3, 2),
		new THREE.Face3(26, 27, 25),				//face 4
		new THREE.Face3(27, 28, 25),
		new THREE.Face3(20, 22, 21),			//Face 6
		new THREE.Face3(20, 23, 22),

		//Car front
		new THREE.Face3(3, 4, 9),
		new THREE.Face3(4, 7, 9),
		new THREE.Face3(3, 9, 6),
		new THREE.Face3(3, 6, 5),
		new THREE.Face3(4, 8, 7),
		new THREE.Face3(6, 9, 7),
		new THREE.Face3(1, 10, 4),
		new THREE.Face3(2, 10, 1),
		new THREE.Face3(11, 10, 2),
		new THREE.Face3(11, 2, 3),
		new THREE.Face3(5, 11, 3),
		new THREE.Face3(4, 10, 8),


		//Left Side
		new THREE.Face3(14, 16, 12),
		new THREE.Face3(16, 18, 12),
		new THREE.Face3(12, 18, 19),
		new THREE.Face3(12, 19, 13),
		new THREE.Face3(13, 19, 17),
		new THREE.Face3(13, 17, 15),

		new THREE.Face3(5, 20, 11),
		new THREE.Face3(20, 21, 11),
		new THREE.Face3(11, 21, 22),
		new THREE.Face3(11, 22, 24),
		new THREE.Face3(22, 23, 24),			//Face 34
		new THREE.Face3(5, 23, 20),
		new THREE.Face3(5, 24, 23),				//Face 38

		//Right Side
		new THREE.Face3(10, 25, 8),
		new THREE.Face3(29, 28, 27),				//face 40
		new THREE.Face3(25, 28, 8),
		new THREE.Face3(28, 29, 8),

		//Back Top
		new THREE.Face3(11, 35, 10),
		new THREE.Face3(24, 35, 11),
		new THREE.Face3(29, 35, 24),
		new THREE.Face3(10, 35, 29),

		//Back Under
		new THREE.Face3(30, 34, 31),
		new THREE.Face3(32, 34, 30),
		new THREE.Face3(33, 34, 32),
		new THREE.Face3(31, 34, 33),


		//Doors
		new THREE.Face3(17, 32, 24),
		new THREE.Face3(17, 24, 15),
		new THREE.Face3(15, 24, 5),
		new THREE.Face3(17,30,24),
		new THREE.Face3(14,8,29),
		new THREE.Face3(16, 14, 29),
		new THREE.Face3(16, 29, 31),
		new THREE.Face3(16, 31, 33),
		new THREE.Face3(27, 26, 10),
		new THREE.Face3(10, 26, 25),
		new THREE.Face3(29,27,10),

		//Lights
		new THREE.Face3(29, 30, 31),
		new THREE.Face3(29, 24, 30),
		new THREE.Face3(8, 14, 12),		//Face 58
		new THREE.Face3(8, 12, 7),
		new THREE.Face3(7, 12, 13),
		new THREE.Face3(7, 13, 6),
		new THREE.Face3(13, 15, 5),
		new THREE.Face3(13, 5, 6),		//Face 63
		);



	for (var i = 0; i < 8; i++)
		carGeometry.faces[i].materialIndex = 0;
	for (; i < 56; i++)
		carGeometry.faces[i].materialIndex = 1;
	for (; i < 64; i++)
		carGeometry.faces[i].materialIndex = 2;

	carGeometry.computeFaceNormals();
	carGeometry.computeVertexNormals();
	base.add(new THREE.Mesh(this.carGeometry, carGeometry.materials));

	return base;
}

class Stage{
	constructor(scene){
		this.podium = new THREE.Object3D()
		scene.add(this.podium);

		var planeGeo = new THREE.PlaneGeometry(100, 100);
		var material = new THREE.MeshBasicMaterial( {color: 0x4a4a4a, side: THREE.DoubleSide});
		var plane = new THREE.Mesh(planeGeo, material);
		plane.rotateX(Math.PI/2);
		scene.add(plane);

		var geometry = new THREE.CylinderGeometry(podiumRadius, podiumRadius, 1, 50);
		var podium = new THREE.Mesh(geometry, floorMaterialBasic);
		podium.translateY(0.5);
		this.podium.add(podium);

		this.cyberTruck = createCybertruck();
		this.podium.add(this.cyberTruck);


		this.spotlights = []

		var target = new THREE.Vector3(0, 0, 0);
		var position = new THREE.Vector3(-10, 13, 0);
		this.spotlights.push(new Spotlight(scene, position, target));

		target = new THREE.Vector3(-2, 0, -3);
		position = new THREE.Vector3(10, 13, 0);
		this.spotlights.push(new Spotlight(scene, position, target));

		target = new THREE.Vector3(2, 0, 3);
		position = new THREE.Vector3(10, -13, 10);
		this.spotlights.push(new Spotlight(scene, position, target));

		this.light = new DirectionalLight(scene);

		this.light_state = "basic";
		this.shadow_state = "phong";

	}

	rotate(direction){
		this.podium.rotateY(0.02*direction);
	}

	toggleSpotlight(index){
		this.spotlights[index].toggle();
	}
	rotateSpotlight(index){
		this.spotlights[index].rotate();
	}
	toggleDirectionalLight(){
		this.light.toggle();
	}

	toggleLightCalculation(){
        console.log("change all the materials to basic materials");

        this.light_state = this.light_state == "basic" ? this.shadow_state : "basic";
        this._setMaterials(this.light_state);
    }

	toggleGouraudPhong(){
        console.log("change from lambert to phone and vice-versa");
        this.shadow_state = this.shadow_state == "phong" ? "lambert" : "phong"
        this._setMaterials(this.shadow_state);
	}

	_setMaterials(state){
        if (state == "basic"){
            console.log("changed to baisc");
            this.podium.children[0].material = floorMaterialBasic;
			for (var i = 0; i < 8; i++)
				this.cyberTruck.children[11].geometry.faces[i].materialIndex = 0;
			for (; i < 56; i++)
				this.cyberTruck.children[11].geometry.faces[i].materialIndex = 1;
			for (; i < 64; i++)
				this.cyberTruck.children[11].geometry.faces[i].materialIndex = 2;

			this.cyberTruck.children[0].material = wheelMaterialBasic;
			this.cyberTruck.children[1].material = wheelMaterialBasic;
			this.cyberTruck.children[2].material = wheelMaterialBasic;
			this.cyberTruck.children[3].material = wheelMaterialBasic;

			this.cyberTruck.children[4].material = greyMaterialBasic;
			this.cyberTruck.children[5].material = greyMaterialBasic;
			this.cyberTruck.children[6].material = greyMaterialBasic;
			this.cyberTruck.children[9].material = greyMaterialBasic;
			this.cyberTruck.children[10].material = greyMaterialBasic;

			this.cyberTruck.children[11].geometry.groupsNeedUpdate = true;
        }
        else if (state == "phong"){
			console.log(this.cyberTruck);
            console.log("changed to phong");
            this.podium.children[0].material = floorMaterialPhong;
			for (var i = 0; i < 8; i++)
				this.cyberTruck.children[11].geometry.faces[i].materialIndex = 3;
			for (; i < 56; i++)
				this.cyberTruck.children[11].geometry.faces[i].materialIndex = 4;
			for (; i < 64; i++)
				this.cyberTruck.children[11].geometry.faces[i].materialIndex = 5;

			this.cyberTruck.children[0].material = wheelMaterialPhong;
			this.cyberTruck.children[1].material = wheelMaterialPhong;
			this.cyberTruck.children[2].material = wheelMaterialPhong;
			this.cyberTruck.children[3].material = wheelMaterialPhong;

			this.cyberTruck.children[4].material = greyMaterialPhong;
			this.cyberTruck.children[5].material = greyMaterialPhong;
			this.cyberTruck.children[6].material = greyMaterialPhong;
			this.cyberTruck.children[9].material = greyMaterialPhong;
			this.cyberTruck.children[10].material = greyMaterialPhong;

			this.cyberTruck.children[11].geometry.groupsNeedUpdate = true;
        }
        else if( state == "lambert"){
            console.log("changed to lambert");
            this.podium.children[0].material = floorMaterialLambert;
			for (var i = 0; i < 8; i++)
				this.cyberTruck.children[11].geometry.faces[i].materialIndex = 6;
			for (; i < 56; i++)
				this.cyberTruck.children[11].geometry.faces[i].materialIndex = 7;
			for (; i < 64; i++)
				this.cyberTruck.children[11].geometry.faces[i].materialIndex = 8;

			this.cyberTruck.children[0].material = wheelMaterialLambert;
			this.cyberTruck.children[1].material = wheelMaterialLambert;
			this.cyberTruck.children[2].material = wheelMaterialLambert;
			this.cyberTruck.children[3].material = wheelMaterialLambert;

			this.cyberTruck.children[4].material = greyMaterialLambert;
			this.cyberTruck.children[5].material = greyMaterialLambert;
			this.cyberTruck.children[6].material = greyMaterialLambert;
			this.cyberTruck.children[9].material = greyMaterialLambert;
			this.cyberTruck.children[10].material = greyMaterialLambert;

			this.cyberTruck.children[11].geometry.groupsNeedUpdate = true;
        }
    }
}

class DirectionalLight{
	constructor(scene){
		this.directionalLight = new THREE.DirectionalLight(0xffffff, 0.5);
		scene.add(this.directionalLight);
	}

	toggle(){
		this.directionalLight.visible = !this.directionalLight.visible;
	}
}

class Spotlight{
	constructor(base, position, target){
		this.base = new THREE.Object3D();
		base.add(this.base);

		//cone
		var visible_base = new THREE.Object3D();
		var geometry = new THREE.CylinderGeometry(0, 0.5, 2, 2);
		var material = new THREE.MeshBasicMaterial({color: 0x32f34, wireframe: false});
		var mesh = new THREE.Mesh(geometry, material);
		visible_base.add(mesh);

		//esfera
		geometry = new THREE.SphereGeometry(0.5);
		material = new THREE.MeshBasicMaterial({color: 0x3f3354, wireframe: false});
		mesh = new THREE.Mesh(geometry, material);
		mesh.translateY(-1);
		visible_base.add(mesh);
		this.base.add(visible_base);

		//luz
		this.spotlight = new THREE.SpotLight( 0xffffff, 0.5, 0, Math.PI / 6, 10);
		this.spotlight.position.set(position.x, position.y, position.z);
		this.spotlight.target.position.set(target.x, target.y, target.z);
		this.spotlight.penumbra = .2;
		this.base.add(this.spotlight);
		this.base.add(this.spotlight.target);

		this.base.position.set(position.x, position.y, position.z);

		visible_base.lookAt(target);
		visible_base.rotateX(Math.PI/2);
		visible_base.rotateZ(Math.PI);
	}

	toggle(){
		this.spotlight.visible = !this.spotlight.visible;
	}
}
