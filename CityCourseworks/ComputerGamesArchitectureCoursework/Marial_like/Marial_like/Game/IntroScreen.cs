using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;

namespace MarialLike2D
{
    public class IntroScreen : DrawableGameComponent
    {
        private ScoreScreen scoreScreen; // Field to hold reference to ScoreScreen instance
        private SpriteBatch spriteBatch;
        private Texture2D introBackground;
        private Texture2D startButton;
        private Texture2D ScoreButton;

        private Rectangle startButtonRect;
        private Rectangle ScoreButtonRect;

        public event EventHandler StartGameClicked;


        public IntroScreen(Game game) : base(game)
        {
        }

        protected override void LoadContent()
        {
            spriteBatch = new SpriteBatch(GraphicsDevice);

            introBackground = Game.Content.Load<Texture2D>("Content/Intro/intro_background");
            startButton = Game.Content.Load<Texture2D>("Content/Intro/start_button");
            ScoreButton = Game.Content.Load<Texture2D>("Content/Intro/score_button");


            // Position the start button at the center of the screen
            startButtonRect = new Rectangle(
                (GraphicsDevice.Viewport.Width - startButton.Width) / 2,
                (GraphicsDevice.Viewport.Height - startButton.Height) / 2,
                startButton.Width,
                startButton.Height);


            // Position the start button at the center of the screen
            ScoreButtonRect = new Rectangle(
                (GraphicsDevice.Viewport.Width - ScoreButton.Width) / 2,
                (GraphicsDevice.Viewport.Height - ScoreButton.Height) / 2 + 200,
                ScoreButton.Width,
                ScoreButton.Height);

            base.LoadContent();
        }




        public override void Update(GameTime gameTime)
        {
            // Check for enter pressed to start the game
            if (Keyboard.GetState().IsKeyDown(Keys.Enter))
            {
                OnStartGameClicked();
            }

            // Check if the score button is clicked
            if (Keyboard.GetState().IsKeyDown(Keys.F1))
            {
                // Create a new ScoreScreen instance
                scoreScreen = new ScoreScreen(Game);
                // Add the score screen component
                Game.Components.Add(scoreScreen);
            }
            /*
            if (Mouse.GetState().LeftButton == ButtonState.Pressed &&
                startButtonRect.Contains(Mouse.GetState().Position))
            {
                OnStartGameClicked();
            }
            */

            base.Update(gameTime);
        }

        public override void Draw(GameTime gameTime)
        {
            spriteBatch.Begin();

            // Draw intro background
            spriteBatch.Draw(introBackground, Vector2.Zero, Color.White);

            // Draw start button
            spriteBatch.Draw(startButton, startButtonRect, Color.White);

            // draw score button
            spriteBatch.Draw(ScoreButton, ScoreButtonRect, Color.Blue);

            spriteBatch.End();

            base.Draw(gameTime);
        }

        protected virtual void OnStartGameClicked()
        {
            StartGameClicked?.Invoke(this, EventArgs.Empty);
        }


    }
}
