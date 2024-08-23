using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace MarialLike2D
{
    public class WinningScreen : DrawableGameComponent
    {
        private SpriteBatch spriteBatch;
        private SpriteFont font;
        private Texture2D background;
        private int score;

        public WinningScreen(Game game, int score) : base(game)
        {
            this.score = score;
        }

        protected override void LoadContent()
        {
            spriteBatch = new SpriteBatch(GraphicsDevice);
            font = Game.Content.Load<SpriteFont>("Fonts/Hud");
            background = Game.Content.Load<Texture2D>("Backgrounds/WinGameBG");
        }

        public override void Draw(GameTime gameTime)
        {
            spriteBatch.Begin();

            // Draw background
            spriteBatch.Draw(background, Vector2.Zero, Color.White);

            // Draw score
            string scoreText = "Score: " + score.ToString();
            Vector2 scoreSize = font.MeasureString(scoreText);
            Vector2 scorePosition = new Vector2((GraphicsDevice.Viewport.Width - scoreSize.X) / 2, GraphicsDevice.Viewport.Height / 2);
            spriteBatch.DrawString(font, scoreText, scorePosition, Color.White);

            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
