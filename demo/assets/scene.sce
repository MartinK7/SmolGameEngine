
# Scene file design in progress

model boxes {
    file models/test_scene/boxes.obj
}
model bulb {
    file models/test_scene/bulb.obj
}
model lightFrame {
    file models/test_scene/light_frame.obj
}
model floor {
    file models/test_scene/floor.obj
}
model walls {
    file models/test_scene/walls.obj
}
model vent {
    file models/test_scene/vent.obj
}
model ventPipe {
    file models/test_scene/vent_pipe.obj
}
model ventHold {
    file models/test_scene/vent_hold.obj
}
model ventProp {
    file models/test_scene/vent_prop.obj
}
model teapot {
    file models/test_scene/teapot.obj
}

texture woodBox {
    file images/textures/TexturesCom_Cargo0097/TexturesCom_Cargo0097_M.jpg
}
texture metalBare {
    file images/textures/TexturesCom_MetalBare0212_7_M.jpg
}
texture concrete {
    file images/textures/TexturesCom_ConcreteBare0330_7_seamless_S.png
}
texture brickRustAlbedo {
    file images/textures/TexturesCom_Brick_Rustic2_1K/TexturesCom_Brick_Rustic2_1K_albedo.png
}
texture brickRustNormal {
    file images/textures/TexturesCom_Brick_Rustic2_1K/TexturesCom_Brick_Rustic2_1K_normal.png
}

cubemap skybox {
    filePositiveX images/cubemaps/Maskonaive2/posx.jpg
    fileNegativeX images/cubemaps/Maskonaive2/negx.jpg
    filePositiveY images/cubemaps/Maskonaive2/posy.jpg
    fileNegativeY images/cubemaps/Maskonaive2/negy.jpg
    filePositiveZ images/cubemaps/Maskonaive2/posz.jpg
    fileNegativeZ images/cubemaps/Maskonaive2/negz.jpg
}

material mat {
    color 0 0 0
    glossy 0
    textureAlbedo brickRustAlbedo
    textureNormal brickRustNormal
    textureHeight brickRustHeight
    textureRoughness brickRustRoughness
    textureAmbientOcclusion brickAmbientOcclusion
    diffuse 1
    specular 2
}

gameObject teapot {
    model teapot
    material mat
    position 0 0 0
    rotation 0 0 0
    scale 0 0 0
}
