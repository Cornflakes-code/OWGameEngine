#include "ComputeNormals.h"

/*
// Add a normal to the list if the smoothing group bits overlap,
// otherwise create a new vertex normal in the list
void VNormal::AddNormal(glm::vec3& n, unsigned int s) {
    if (!(s & smooth) && init) {
        if (next) next->AddNormal(n, s);
        else {
            next = new VNormal(n, s);
        }
    }
    else {
        norm += n;
        smooth |= s;
        init = true;
    }
}

// Retrieves a normal if the smoothing groups overlap or there is
// only one in the list
glm::vec3& VNormal::GetNormal(unsigned int s)
{
    if (smooth & s || !next) return norm;
    else return next->GetNormal(s);
}

// Normalize each normal in the list
void VNormal::Normalize() {
    VNormal* ptr = next, * prev = this;
    while (ptr)
    {
        if (ptr->smooth & smooth) {
            norm += ptr->norm;
            smooth |= ptr->smooth;
            prev->next = ptr->next;
            delete ptr;
            ptr = prev->next;
        }
        else {
            prev = ptr;
            ptr = ptr->next;
        }
    }
    norm = ::Normalize(norm);
    if (next) next->Normalize();
}

// Compute the face and vertex normals
void Utility::ComputeVertexNormals(Mesh* mesh)
{
    Face* face;
    glm::vec3* verts;
    glm::vec3 v0, v1, v2;
    Tab<VNormal> vnorms;
    Tab<glm::vec3> fnorms;
    face = mesh->faces;
    verts = mesh->verts;
    vnorms.SetCount(mesh->getNumVerts());
    fnorms.SetCount(mesh->getNumFaces());

    // Compute face and vertex surface normals
    for (int i = 0; i < mesh->getNumVerts(); i++) {
        vnorms[i] = VNormal();
    }
    for (int i = 0; i < mesh->getNumFaces(); i++, face++) {
        // Calculate the surface normal
        v0 = verts[face->v[0]];
        v1 = verts[face->v[1]];
        v2 = verts[face->v[2]];
        fnorms[i] = (v1 - v0) ^ (v2 - v1);
        for (int j = 0; j < 3; j++) {
            vnorms[face->v[j]].AddNormal(fnorms[i], face->smGroup);
        }
        fnorms[i] = Normalize(fnorms[i]);
    }
    for (i = 0; i < mesh->getNumVerts(); i++) {
        vnorms[i].Normalize();
    }
    // Display the normals in the debug window of the VC++ IDE
    DebugPrint(";\n\nVertex Normals ---";);
    for (i = 0; i < vnorms.Count(); i++) {
        DisplayVertexNormal(vnorms.Addr(i), i, 0);
    }
    DebugPrint("\n\n");
}

void Utility::DisplayVertexNormal(VNormal* vn, int i, int n)
{
    DebugPrint("\nVertex %d Normal %d=(%.1f, %.1f, %.1f)&",
        i, n, vn->norm.x, vn->norm.y, vn->norm.z);
    if (vn->next) DisplayVertexNormal(vn->next, i, n + 1);
}
*/
