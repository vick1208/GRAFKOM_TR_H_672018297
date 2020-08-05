#pragma once
#include <math.h>

class Nfg {

	struct vertex {
		_Vec3 pos, norm, binorm, tgt;
		_Vec2 uv;
	};

	struct pattern {
		_VecInt3 pos;
	};

public:
	struct vertex myVertex[512];
	struct pattern myPattern[718];
	GLint totalNrVertex, totalNrIndices;

private:
	FILE* loadModel;

	float px, py, pz, nx, ny, nz, bx, by, bz, tx, ty, tz, u, v;
	int x, y, z;
	char junk[200];

	bool loadFile() {
		// Buka File
		fopen_s(&loadModel, "Woman1.nfg", "r");
		if (!loadModel) {
			return false;
		}
		return true;
	}

	void loadNrVertex() {
		// Mendapatkan total node
		fscanf_s(this->loadModel, "NrVertices : %d", &totalNrVertex);
		//printf("NrVertices:%d\n", totalNrVertex);
	}

	void loadCoordinate() {
		for (int i = 0; i < totalNrVertex; i++) {

			// Membaca nilai pada setiap baris 
			fscanf_s(loadModel, "%*d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f];", &px, &py, &pz, &nx, &ny, &nz, &bx, &by, &bz, &tx, &ty, &tz, &u, &v);

			this->myVertex[i].pos.x = px;
			this->myVertex[i].pos.y = py;
			this->myVertex[i].pos.z = pz;

			this->myVertex[i].norm.x = nx;
			this->myVertex[i].norm.y = ny;
			this->myVertex[i].norm.z = nz;

			this->myVertex[i].binorm.x = bx;
			this->myVertex[i].binorm.y = by;
			this->myVertex[i].binorm.z = bz;

			this->myVertex[i].tgt.x = tx;
			this->myVertex[i].tgt.y = ty;
			this->myVertex[i].tgt.z = tz;

			this->myVertex[i].uv.x = u;
			this->myVertex[i].uv.y = v;

			//printf("%d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f];\n", i, px, py, pz, nx, ny, nz, bx, by, bz, tx, ty, tz, u, v);
		}
	}

	void loadNrIndices() {
		fscanf_s(this->loadModel, "\nNrIndices:%d", &totalNrIndices);
		//printf("NrIndices:%d\n", totalNrIndices);
	}

	void trianglePattern() {

		int Ntriangle = totalNrIndices / 3;

		fscanf_s(this->loadModel, "\n");

		for (int i = 0; i < Ntriangle; i++) {
			fscanf_s(this->loadModel, "%*d. %d, %d, %d", &x, &y, &z);
			//printf("%d. %d %d %d\n", i, x, y, z);
			this->myPattern[i].pos.x = x;
			this->myPattern[i].pos.y = y;
			this->myPattern[i].pos.z = z;

			//printf("%d. %d %d %d\n", i, this->myPattern[i].pos.x, this->myPattern[i].pos.y, this->myPattern[i].pos.z);

		}

	}

	void closeFile() {
		fclose(loadModel);
	}

public:
	void process() {
		if (this->loadFile()) {
			this->loadNrVertex();
			this->loadCoordinate();
			this->loadNrIndices();
			this->trianglePattern();
			this->closeFile();
		}
		else {
			printf("File cannot proccessed");
		}
	}
};
