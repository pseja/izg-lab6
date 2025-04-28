#ifdef VERTEX_SHADER
#line 2
precision highp float;

uniform mat4 view;
uniform mat4 proj;

// output for fragment shader
flat out int export_block_id;

vec3 rotateAroundX(vec3 point, float angle)
{
    float s = sin(angle);
    float c = cos(angle);

    mat3 rotation_matrix = mat3(
        1.0, 0.0, 0.0,
        0.0, c,   -s, 
        0.0, s,   c
    );

    return rotation_matrix * point;
}

vec3 rotateAroundY(vec3 point, float angle)
{
    float s = sin(angle);
    float c = cos(angle);

    mat3 rotation_matrix = mat3(
        c,   0.0, -s,
        0.0, 1.0, 0.0, 
        s,   0.0, c
    );

    return rotation_matrix * point;
}

void main(){
    gl_Position = vec4(0.f,0.f,0.f,1.f);

    uint indices[] = uint[](
        0u,1u,2u,2u,1u,3u
    );

    const int total_triangles = indices.length() * 6;
    int block_id = gl_VertexID / indices.length();

    export_block_id = block_id;

    int local_vertex_id = gl_VertexID % indices.length();

    if(gl_VertexID >= total_triangles)
    {
        return;
    }

    vec3 pos;
    for(uint i = 0u; i < 3u; i++)
    {
        pos[i] = float((indices[local_vertex_id] >> i) & 1u);
    }

    vec3 scale;
    vec3 offset;
    vec3 final_pos;

    // front
    if (block_id == 0)
    {
        scale = vec3(1.0, 1.0, 0.0);
        offset = vec3(0.0, 0.0, 0.0);
        final_pos = (pos * 2.0 - 1.0) * scale + offset;
    }
    // back
    else if (block_id == 1)
    {
        scale = vec3(1.0, 1.0, 0.0);
        offset = vec3(0.0, 0.0, 2.0);
        final_pos = (pos * 2.0 - 1.0) * scale + offset;
    }
    // up
    else if (block_id == 2)
    {
        scale = vec3(1.0, 1.0, 0.0);
        vec3 scaled_pos = (pos * 2.0 - 1.0) * scale;
        scaled_pos = rotateAroundX(scaled_pos, radians(90.0));
        offset = vec3(0.0, 1.0, 1.0);
        final_pos = scaled_pos + offset;
    }
    // bottom
    else if (block_id == 3)
    {
        scale = vec3(1.0, 1.0, 0.0);
        vec3 scaled_pos = (pos * 2.0 - 1.0) * scale;
        scaled_pos = rotateAroundX(scaled_pos, radians(90.0));
        offset = vec3(0.0, -1.0, 1.0);
        final_pos = scaled_pos + offset;
    }
    // left
    else if (block_id == 4)
    {
        scale = vec3(1.0, 1.0, 0.0);
        vec3 scaled_pos = (pos * 2.0 - 1.0) * scale;
        scaled_pos = rotateAroundY(scaled_pos, radians(90.0));
        offset = vec3(-1.0, 0.0, 1.0);
        final_pos = scaled_pos + offset;
    }
    // right
    else if (block_id == 5)
    {
        scale = vec3(1.0, 1.0, 0.0);
        vec3 scaled_pos = (pos * 2.0 - 1.0) * scale;
        scaled_pos = rotateAroundY(scaled_pos, radians(-90.0));
        offset = vec3(1.0, 0.0, 1.0);
        final_pos = scaled_pos + offset;
    }

    gl_Position = proj * view * vec4(final_pos * 2.0f - 1.f, 1.f);
}
#endif



#ifdef FRAGMENT_SHADER
precision highp float;
precision highp float;

out highp vec4 fColor;

flat in int export_block_id;

void main()
{
    if (export_block_id == 0)
    {
        fColor = vec4(1.0, 0.0, 0.0, 1.0);
    } 
    else if (export_block_id == 1)
    {
        fColor = vec4(0.0, 1.0, 0.0, 1.0);
    }
    else if (export_block_id == 2)
    {
        fColor = vec4(0.0, 0.0, 1.0, 1.0);
    }
    else if (export_block_id == 3)
    {
        fColor = vec4(1.0, 1.0, 0.0, 1.0);
    }
    else if (export_block_id == 4)
    {
        fColor = vec4(1.0, 0.0, 1.0, 1.0);
    }
    else if (export_block_id == 5)
    {
        fColor = vec4(0.0, 1.0, 1.0, 1.0);
    }
    else
    {
        fColor = vec4(0.0, 1.0, 0.0, 1.0);
    }
}
#endif
