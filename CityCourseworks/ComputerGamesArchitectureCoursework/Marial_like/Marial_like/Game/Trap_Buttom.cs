using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Graphics;

namespace MarialLike2D
{

    /// <summary>
    /// A trap at the buttom of the scene to make player death when attach
    /// </summary>
    class Trap_Buttom
    {
        private Texture2D texture;
        private Vector2 origin;

        public readonly Color color = Color.Gray;

        private Vector2 basePosition;
        public Level Level
        {
            get { return level; }
        }
        Level level;

        /// <summary>
        /// Position in world space of the bottom center of this trap.
        /// </summary>
        public Vector2 Position
        {
            get { return basePosition; }
        }

        private Rectangle localBounds;
        /// <summary>
        /// Gets a circle which bounds this trap in world space.
        /// </summary>
        public Circle BoundingCircle
        {
            get
            {
                return new Circle(Position, Tile.Width / 3.0f);
            }
        }


        public Trap_Buttom(Level level, Vector2 position, string spriteSet)
        {
            this.level = level;
            this.basePosition = position;

            LoadContent(spriteSet);
        }

        /// <summary>
        /// Loads a particular enemy sprite sheet and sounds.
        /// </summary>
        public void LoadContent(string spriteSet)
        {
            spriteSet = "Sprites/" + spriteSet + "/";
            texture = Level.Content.Load<Texture2D>(spriteSet + "Trap_Buttom");
            origin = new Vector2(texture.Width / 2.0f, texture.Height / 2.0f);

        }


        /// <summary>
        /// Paces back and forth along a platform, waiting at either end.
        /// </summary>
        public void Update(GameTime gameTime)
        {

        }

        /// <summary>
        /// Draws the animated enemy.
        /// </summary>
        public void Draw(GameTime gameTime, SpriteBatch spriteBatch, float sceneOffsetY)
        {
            spriteBatch.Draw(texture, Position + new Vector2(0, sceneOffsetY), null, color, 0.0f, origin, 1.0f, SpriteEffects.None, 0.0f);
        }
    }
}