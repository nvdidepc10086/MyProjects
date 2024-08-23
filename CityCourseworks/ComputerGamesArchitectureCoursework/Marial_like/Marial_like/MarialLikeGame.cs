
using System;
using System.IO;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;
using Microsoft.Xna.Framework.Content;

#if !__IOS__
using Microsoft.Xna.Framework.Media;
#endif

namespace MarialLike2D
{

    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class MarialLikeGame : Microsoft.Xna.Framework.Game
    {
        private enum GameState
        {
            Intro,
            Playing,
            EndGame,
            Win
        }
        private GameState gameState;
        private IntroScreen introScreen;
        private EndGameScreen endGameScreen;
        private WinningScreen winningScreen;
        private int currentScore;

        // Add a field to store the current Y-offset of the gameplay scene
        private float sceneOffsetY = 0;

        // Adjust the speed at which the scene moves down by changing this value
        private float sceneMoveSpeed = 1f; // You can adjust this value to control the speed

        // Resources for drawing.
        private GraphicsDeviceManager graphics;
        private SpriteBatch spriteBatch;
        Vector2 baseScreenSize = new Vector2(800, 480);
        private Matrix globalTransformation;
        int backbufferWidth, backbufferHeight;

        // Global content.
        private SpriteFont hudFont;

        private Texture2D winOverlay;
        private Texture2D loseOverlay;
        private Texture2D diedOverlay;

        // Meta-level game state.
        private int levelIndex = -1;
        private Level level;
        private bool wasContinuePressed;

        // When the time remaining is less than the warning time, it blinks on the hud
        private static readonly TimeSpan WarningTime = TimeSpan.FromSeconds(30);

        // We store our input states so that we only poll once per frame, 
        // then we use the same input state wherever needed
        private GamePadState gamePadState;
        private KeyboardState keyboardState;
        private TouchCollection touchState;
        private AccelerometerState accelerometerState;

        private VirtualGamePad virtualGamePad;

        // The number of levels in the Levels directory of our content. We assume that
        // levels in our content are 0-based and that all numbers under this constant
        // have a level file present. This allows us to not need to check for the file
        // or handle exceptions, both of which can add unnecessary time to level loading.
        private const int numberOfLevels = 1;

        public MarialLikeGame()
        {
            graphics = new GraphicsDeviceManager(this);

#if WINDOWS_PHONE
            TargetElapsedTime = TimeSpan.FromTicks(333333);
#endif
            graphics.IsFullScreen = false;

            //graphics.PreferredBackBufferWidth = 800;
            //graphics.PreferredBackBufferHeight = 480;
            graphics.SupportedOrientations = DisplayOrientation.LandscapeLeft | DisplayOrientation.LandscapeRight;

            Accelerometer.Initialize();
        }

        protected override void Initialize()
        {
            gameState = GameState.Intro;
            introScreen = new IntroScreen(this);
            introScreen.StartGameClicked += IntroScreen_StartGameClicked;
            Components.Add(introScreen);

            base.Initialize();
        }

        /// <summary>
        /// LoadContent will be called once per game and is the place to load
        /// all of your content.
        /// </summary>
        protected override void LoadContent()
        {
            this.Content.RootDirectory = "Content";

            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);

            // Load fonts
            hudFont = Content.Load<SpriteFont>("Fonts/Hud");

            // Load overlay textures
            winOverlay = Content.Load<Texture2D>("Overlays/next_level");
            loseOverlay = Content.Load<Texture2D>("Overlays/time_up");
            diedOverlay = Content.Load<Texture2D>("Overlays/died");

            ScalePresentationArea();

            virtualGamePad = new VirtualGamePad(baseScreenSize, globalTransformation, Content.Load<Texture2D>("Sprites/VirtualControlArrow"));

#if !__IOS__
            //Known issue that you get exceptions if you use Media PLayer while connected to your PC
            //See http://social.msdn.microsoft.com/Forums/en/windowsphone7series/thread/c8a243d2-d360-46b1-96bd-62b1ef268c66
            //Which means its impossible to test this from VS.
            //So we have to catch the exception and throw it away
            try
            {
                MediaPlayer.IsRepeating = true;
                MediaPlayer.Play(Content.Load<Song>("Sounds/Music"));
            }
            catch { }
#endif
            LoadNextLevel();
        }

        public void ScalePresentationArea()
        {
            //Work out how much we need to scale our graphics to fill the screen
            backbufferWidth = GraphicsDevice.PresentationParameters.BackBufferWidth;
            backbufferHeight = GraphicsDevice.PresentationParameters.BackBufferHeight;
            float horScaling = backbufferWidth / baseScreenSize.X;
            float verScaling = backbufferHeight / baseScreenSize.Y;
            Vector3 screenScalingFactor = new Vector3(horScaling, verScaling, 1);
            globalTransformation = Matrix.CreateScale(screenScalingFactor);
            System.Diagnostics.Debug.WriteLine("Screen Size - Width[" + GraphicsDevice.PresentationParameters.BackBufferWidth + "] Height [" + GraphicsDevice.PresentationParameters.BackBufferHeight + "]");
        }


        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {

            switch (gameState)
            {
                case GameState.Intro:
                    // No need to update game logic while in the intro screen
                    break;
                case GameState.Playing:
                    // Update game logic...
                    // Check for player death
                    if (!level.Player.IsAlive || level.TimeRemaining == TimeSpan.Zero)
                    {
                        gameState = GameState.EndGame;
                        currentScore = level.Score;
                        sceneOffsetY = 0;
                        endGameScreen = new EndGameScreen(this, currentScore);
                        Components.Add(endGameScreen);
                    }
                    else
                    {
                        // Handle game playing logic...
                        //Confirm the screen has not been resized by the user
                        if (backbufferHeight != GraphicsDevice.PresentationParameters.BackBufferHeight ||
                            backbufferWidth != GraphicsDevice.PresentationParameters.BackBufferWidth)
                        {
                            ScalePresentationArea();
                        }
                        // Handle polling for our input and handling high-level input
                        HandleInput(gameTime);

                        // Move the gameplay scene down
                        sceneOffsetY -= sceneMoveSpeed;

                        // update our level, passing down the GameTime along with all of our input states
                        level.Update(gameTime, keyboardState, gamePadState,
                                     accelerometerState, Window.CurrentOrientation);

                        if (level.Player.Velocity != Vector2.Zero)
                            virtualGamePad.NotifyPlayerIsMoving();
                    }

                    if (level.ReachedExit)
                    {
                        gameState = GameState.Win;
                        currentScore = level.Score;
                        sceneOffsetY = 0;
                        winningScreen = new WinningScreen(this, currentScore);
                        Components.Add(winningScreen);

                    }
                    break;

                case GameState.EndGame:
                    // Handle end game logic...

                    // Check for SPACE key press to return to intro screen
                    if (Keyboard.GetState().IsKeyDown(Keys.Space))
                    {
                        gameState = GameState.Intro;
                        Components.Remove(endGameScreen);
                        endGameScreen.Dispose();
                        introScreen = new IntroScreen(this);
                        introScreen.StartGameClicked += IntroScreen_StartGameClicked;
                        Components.Add(introScreen);
                    }
                    break;

                case GameState.Win:
                    // Check for SPACE key press to return to intro screen
                    if (Keyboard.GetState().IsKeyDown(Keys.Space))
                    {
                        gameState = GameState.Intro;
                        Components.Remove(winningScreen);
                        winningScreen.Dispose();
                        introScreen = new IntroScreen(this);
                        introScreen.StartGameClicked += IntroScreen_StartGameClicked;
                        Components.Add(introScreen);
                    }
                    break;
            }


            base.Update(gameTime);
        }

        private void HandleInput(GameTime gameTime)
        {
            // get all of our input states
            keyboardState = Keyboard.GetState();
            touchState = TouchPanel.GetState();
            gamePadState = virtualGamePad.GetState(touchState, GamePad.GetState(PlayerIndex.One));
            accelerometerState = Accelerometer.GetState();

#if !NETFX_CORE && !__IOS__
            // Exit the game when back is pressed.
            if (gamePadState.Buttons.Back == ButtonState.Pressed)
                Exit();
#endif
            bool continuePressed =
                keyboardState.IsKeyDown(Keys.Space) ||
                gamePadState.IsButtonDown(Buttons.A) ||
                touchState.AnyTouch();

            // Perform the appropriate action to advance the game and
            // to get the player back to playing.
            if (!wasContinuePressed && continuePressed)
            {
                if (!level.Player.IsAlive)
                {
                    level.StartNewLife();
                }
                else if (level.TimeRemaining == TimeSpan.Zero)
                {
                    if (level.ReachedExit)
                        LoadNextLevel();
                    else
                        ReloadCurrentLevel();
                }
            }

            wasContinuePressed = continuePressed;

            virtualGamePad.Update(gameTime);
        }

        private void LoadNextLevel()
        {
            // move to the next level
            levelIndex = (levelIndex + 1) % numberOfLevels;

            // Unloads the content for the current level before loading the next one.
            if (level != null)
                level.Dispose();

            // Load the level.
            string levelPath = string.Format("Content/Levels/{0}.txt", levelIndex);
            using (Stream fileStream = TitleContainer.OpenStream(levelPath))
                level = new Level(Services, fileStream, levelIndex);

        }

        private void ReloadCurrentLevel()
        {
            --levelIndex;
            LoadNextLevel();
        }

        /// <summary>
        /// Draws the game from background to foreground.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {

            graphics.GraphicsDevice.Clear(Color.CornflowerBlue);

            switch (gameState)
            {
                case GameState.Intro:
                    // No need to draw game objects while in the intro screen
                    break;
                case GameState.Playing:
                    // Draw game objects...
                    spriteBatch.Begin(SpriteSortMode.Immediate, null, null, null, null, null, globalTransformation);

                    level.Draw(gameTime, spriteBatch, sceneOffsetY);

                    DrawHud();

                    spriteBatch.End();
                    break;
                case GameState.EndGame:
                    // Draw end game screen...
                    spriteBatch.Begin();
                    endGameScreen.Draw(gameTime);
                    spriteBatch.End();
                    break;
                case GameState.Win:
                    // Draw win game screen...
                    spriteBatch.Begin();
                    winningScreen.Draw(gameTime);
                    spriteBatch.End();
                    break;
            }

            base.Draw(gameTime);
        }

        private void DrawHud()
        {
            Rectangle titleSafeArea = GraphicsDevice.Viewport.TitleSafeArea;
            Vector2 hudLocation = new Vector2(titleSafeArea.X, titleSafeArea.Y);
            //Vector2 center = new Vector2(titleSafeArea.X + titleSafeArea.Width / 2.0f,
            //                             titleSafeArea.Y + titleSafeArea.Height / 2.0f);

            Vector2 center = new Vector2(baseScreenSize.X / 2, baseScreenSize.Y / 2);

            // Draw time remaining. Uses modulo division to cause blinking when the
            // player is running out of time.
            string timeString = "TIME: " + level.TimeRemaining.Minutes.ToString("00") + ":" + level.TimeRemaining.Seconds.ToString("00");
            Color timeColor;
            if (level.TimeRemaining > WarningTime ||
                level.ReachedExit ||
                (int)level.TimeRemaining.TotalSeconds % 2 == 0)
            {
                timeColor = Color.Yellow;
            }
            else
            {
                timeColor = Color.Red;
            }
            DrawShadowedString(hudFont, timeString, hudLocation, timeColor);

            // Draw score
            float timeHeight = hudFont.MeasureString(timeString).Y;
            DrawShadowedString(hudFont, "SCORE: " + level.Score.ToString(), hudLocation + new Vector2(0.0f, timeHeight * 1.2f), Color.Yellow);


            // Determine the status overlay message to show.
            Texture2D status = null;
            if (level.TimeRemaining == TimeSpan.Zero)
            {
                if (level.ReachedExit)
                {
                    status = winOverlay;
                }
                else
                {
                    status = loseOverlay;
                }
            }
            else if (!level.Player.IsAlive)
            {
                status = diedOverlay;
            }

            if (status != null)
            {
                // Draw status message.
                Vector2 statusSize = new Vector2(status.Width, status.Height);
                spriteBatch.Draw(status, center - statusSize / 2, Color.White);
            }

            if (touchState.IsConnected)
                virtualGamePad.Draw(spriteBatch);
        }

        private void DrawShadowedString(SpriteFont font, string value, Vector2 position, Color color)
        {
            spriteBatch.DrawString(font, value, position + new Vector2(1.0f, 1.0f), Color.Black);
            spriteBatch.DrawString(font, value, position, color);
        }

        private void IntroScreen_StartGameClicked(object sender, EventArgs e)
        {
            gameState = GameState.Playing;
            Components.Remove(introScreen); // Remove the intro screen component
            introScreen.Dispose(); // Dispose the intro screen
            LoadNextLevel(); // Load the first level
        }
    }
}
