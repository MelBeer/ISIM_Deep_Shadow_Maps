#include "Renderer.h"
#include "Image.h"
#include "printer.h"

using namespace std;

int main() {
    Assimp::Importer importer;

    /*
     * You might want to use a .obj file, but you will not get any camera or lights.
     * You might want to use a Camera and Light classes rather than Assimp's ones.
     */
    const aiScene *scene = importer.ReadFile(
            "scenes/monkey.dae",
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType);

    // TODO : add a camera and a light

    // Just some printing
    for (int i = 0; i < scene->mNumMeshes; ++i) {
        auto mesh = scene->mMeshes[i];

        if (mesh->HasNormals())
            std::cout << mesh->mName.data << " : has normals\n";
        std::cout << mesh->mVertices[0] << " pos 0\n";
        std::cout << mesh->mName.data << " : There are " << mesh->mNumFaces << " faces in this mesh" << '\n';
        std::cout << mesh->mName.data << " : There are " << mesh->mNumVertices << " vertices in this mesh" << '\n';

        for (int j = 0; j < mesh->mNumFaces; ++j) {

            auto n = mesh->mFaces[j];

            if (n.mIndices[0] > mesh->mNumVertices || n.mIndices[1] > mesh->mNumVertices || n.mIndices[2] > mesh->mNumVertices)
                std::cout << "\t face " << j << " points : " << n.mIndices[0] << ", " << n.mIndices[1] << ", " << n.mIndices[2] << '\n';
        }
    }

    // Computation start
    Renderer renderer = Renderer{scene};
    Image img = renderer.renderScene(800, 450);
    img.saveImage("test/monkey.ppm");

    return 0;
}