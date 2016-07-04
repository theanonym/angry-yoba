#include "angry_yoba/cursor.hpp"
#include "pukan/core/services/resources.hpp"

Cursor::Cursor(const sf::Texture * texture)
   : _texture(texture)
{
}

void Cursor::draw(sf::RenderTarget & renderTarget, sf::RenderStates) const
{
   sf::Sprite sprite(*_texture);
   sprite.setPosition(_pos);
   renderTarget.draw(sprite, Resources::get().getShader("blur"));
}


