using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System.IO;
using System;
using System.Collections.Generic;

namespace MarialLike2D
{
    public class ScoreScreen : DrawableGameComponent
    {
        private SpriteFont font;
        private int[] loadedScore;
        private Texture2D ScoreScreenBackground;
        private SpriteBatch spriteBatch;

        public ScoreScreen(Game game) : base(game)
        {
        }

        protected override void LoadContent()
        {

            spriteBatch = new SpriteBatch(GraphicsDevice);
            ScoreScreenBackground = Game.Content.Load<Texture2D>("Intro/score_screen_bg");
            font = Game.Content.Load<SpriteFont>("Fonts/Hud");
            base.LoadContent();
        }

        public override void Update(GameTime gameTime)
        {
            if (Keyboard.GetState().IsKeyDown(Keys.Escape))
            {
                // Go back to the intro screen when ESC is pressed
                Game.Components.Remove(this);
            }

            base.Update(gameTime);
        }

        public override void Draw(GameTime gameTime)
        {
            spriteBatch.Begin();

            // Draw score screen background
            spriteBatch.Draw(ScoreScreenBackground, Vector2.Zero, Color.White);
            // Load the score from file
            loadedScore = LoadScoresFromFile("score.txt");

            for (int i = 0; i < loadedScore.Length; i++)
            {
                //loadedScore[i] = LoadScoreFromFile("score.txt");
                // Draw the loaded score
                string scoreString = "History Score: " + loadedScore[i];
                //Vector2 scorePosition = new Vector2((GraphicsDevice.Viewport.Width - font.MeasureString(scoreString).X) / 2, (GraphicsDevice.Viewport.Height + font.MeasureString(scoreString).Y) * (i + 1) / (i + 2));
                Vector2 scorePosition = new Vector2((GraphicsDevice.Viewport.Width - 
                    font.MeasureString(scoreString).X) / 2, 
                    font.MeasureString(scoreString).Y * (i + 1) + (i + 1) * (GraphicsDevice.Viewport.Height - 
                    (font.MeasureString(scoreString).Y * loadedScore.Length)) / (loadedScore.Length + 1));
                spriteBatch.DrawString(font, scoreString, scorePosition, Color.Red);
            }
            spriteBatch.End();

            base.Draw(gameTime);
        }

        public int[] LoadScoresFromFile(string filePath)
        {
            List<int> scores = new List<int>(); // Initialize list to store scores

            try
            {
                // Open the file for reading
                using (StreamReader reader = new StreamReader(filePath))
                {
                    string line;
                    // Read all lines from the file
                    while ((line = reader.ReadLine()) != null)
                    {
                        // Parse the score string to an integer
                        if (int.TryParse(line, out int score))
                        {
                            scores.Add(score); // Add parsed score to the list
                        }
                        else
                        {
                            Console.WriteLine("Failed to parse score from line: " + line);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                // Handle any exceptions that might occur during file reading
                Console.WriteLine("An error occurred while loading the scores: " + ex.Message);
            }

            return scores.ToArray(); // Return the loaded scores as an array
        }


    }


}
