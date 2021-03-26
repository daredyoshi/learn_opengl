#include <iostream>
#include "Texture.h"
#include "stb_image.h"

Texture::Texture(const std::string texturePath){
    init();
    load(texturePath);
}

Texture::Texture(){
    init();
}

void Texture::load(const std::string texturePath){
    // load the texture into a char stream
    m_path = texturePath;
    int width{0};
    int height{0};
    int nrChannels{0};
    // openGl expects images upside down
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

    if (data){
        // now bind the texture (this makes it the current object other things operate on)
        glBindTexture(GL_TEXTURE_2D, m_ID);
        // now generate the textuer using the previously loaded data
        //
        // The first argument sets the target. This means that GL_TEXTURE_1D or GL_TEXTURE_3D
        // will not be effected by this call
        //
        // The second argument specifies the mipmap level to generate
        // The third argument is what sort of format we want to tsore the textuer in
        // The next two images set the pixel width and height of the resulting texture.
        // The next argument should always be 0 (legacy)
        // The 7th and 8th specify the format and datatype of the source image. We loaded
        // the textuer as RGB values and stored them as chars (bytes) so we'll pass the
        // corresponding values. 
        // The last is the actual image data
        auto const  baseInternalFormat {(nrChannels == 3) ? GL_RGB : GL_RGBA };
            
        glTexImage2D(GL_TEXTURE_2D, 0, baseInternalFormat, width, height, 0, 
                baseInternalFormat, GL_UNSIGNED_BYTE, data);
        // Generate the mipmaps to the level previously specified
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture " << texturePath << '\n';
    }

    // stbi comes with it's own wrappers for releasing pointer data
    stbi_image_free(data); 
}

void Texture::init(){
    // Like any open gl objects, textures are referenced with an id
    // the first input is how many textures to generate and the second
    // is an int array with the ids (only a single id in our case)
    glGenTextures(1, &m_ID); 
}
