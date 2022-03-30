#include "Renderer.h"
#include "Image.h"

using namespace std;

int main() {
    Assimp::Importer importer;

    // TODO : add dynamic path (./scenes/ ...)
    /*
     * You might want to use a .obj file, but you will not get any camera or lights
     * Right now, the file imported is .dae (COLLADA) because it contains the scene elms, but I'm not sure if it works perfectly
     * You might want to use a Camera and Light classes rather than Assimp's ones.
     */
    const aiScene *scene = importer.ReadFile(
            "/home/melbeer/Documents/Epita/Image/Isim/ISIM_Deep_Shadow_Maps/scenes/monkey.dae",
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType);

    // Just some printing
    std::cout << "There are " << scene->mNumCameras << " cameras in the scene" << '\n';

    for (int i = 0; i < scene->mNumMeshes; ++i) {
        auto mesh = scene->mMeshes[i];

        if (mesh->HasNormals())
            std::cout << mesh->mName.data << " : has normals\n";
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
    Image img = renderer.renderScene(400);
    // TODO : add dynamic path (./test/ ...)
    img.saveImage("/home/melbeer/Documents/Epita/Image/Isim/ISIM_Deep_Shadow_Maps/test/monkey.ppm");

    return 0;
}