using System.Collections.Generic;
using System.Linq;
using UnityEngine;

namespace ObjectPermanence
{
    /**
     * Manages the debug console.
     * 
     * Notes:
     * - Add new commands to this class via the AddCommand function.
     * - The inspiration for the design for this console was from "Game Dev Guide" - https://www.youtube.com/watch?v=VzOEM-4A2OM
     */
    public class DebugConsoleManager : Singleton<DebugConsoleManager>
    {
        [System.Serializable]
        private struct ConsoleSettings
        {
            public float Height;
            public float HelpBoxHeight;
            public Color Colour;
        }

        [SerializeField] private ConsoleSettings _consoleSettings;
        [SerializeField] private bool _enableConsole;
        bool _showConsole;
        bool _showHelp;

        private string _input;
        private Vector2 _helpBoxScroll;

        private List<DebugCommandBase> _commandList;

        private DebugConsoleManager()
        {
            _consoleSettings = default;
            _enableConsole = true;
            _showConsole = false;
            _showHelp = false;
            _input = string.Empty;
            _helpBoxScroll = default;
            _commandList = new List<DebugCommandBase>();
        }

        protected override void Awake()
        {
            base.Awake();
            // Note - keep these alphabetical order please :) With a space between the last command and the "help" command

            AddCommand(new DebugCommand<string>("change_scene", "Changes scene to the specified scene", "change_scene [scene name]",
                (sceneName) =>
                {
                    DebugCommandFunctions.ChangeScene(this, sceneName);
                }));
            AddCommand(new DebugCommand("reset_scene", "Resets the current scene", "reset_scene",
                () =>
                {
                    DebugCommandFunctions.ResetScene(this);
                }));

            AddCommand(new DebugCommand("help", "Displays all the available commands, call again to toggle off the display", "help",
                () =>
                {
                    _showHelp = !_showHelp;
                }));
        }

        private void Update()
        {
            if (Input.GetKeyDown(KeyCode.Tilde))
            {
                if (_enableConsole)
                {
                    _showConsole = !_showConsole;

                    // Probs a better way to do this, but this check ensures that the console input key is not added to input
                    if (_showConsole && _input.Length > 0 && _input[_input.Length - 1] == '`')
                    {
                        _input = _input.Remove(_input.Length - 1);
                    }
                }
            }

            if (Input.GetKeyDown(KeyCode.Return))
            {
                if (_enableConsole)
                {
                    if (_showConsole)
                    {
                        HandleInput();
                        _input = string.Empty;
                    }
                }
            }
        }

        private void OnGUI()
        {
            if (_showConsole)
            {
                float y = 0.0f;

                if (_showHelp)
                {
                    GUI.Box(new Rect(0, y, Screen.width, _consoleSettings.HelpBoxHeight), "");

                    Rect viewport = new Rect(0, 0, Screen.width - 30, 20 * _commandList.Count);
                    _helpBoxScroll = GUI.BeginScrollView(new Rect(0, y + 5.0f, Screen.width, 90), _helpBoxScroll, viewport);

                    for (int i = 0; i < _commandList.Count; ++i)
                    {
                        DebugCommandBase command = _commandList[i];
                        string label = $"{command.Format} - {command.Desc}";

                        Rect labelRect = new Rect(5, 20 * i, viewport.width - _consoleSettings.HelpBoxHeight, 20);
                        GUI.Label(labelRect, label);
                    }

                    GUI.EndScrollView();
                    y += _consoleSettings.HelpBoxHeight;
                }

                GUI.Box(new Rect(0, y, Screen.width, _consoleSettings.Height), "");
                GUI.backgroundColor = _consoleSettings.Colour;

                GUI.SetNextControlName("console");
                _input = GUI.TextField(new Rect(10.0f, y + 5.0f, Screen.width - 20.0f, 20.0f), _input);
                GUI.FocusControl("console");
            }
        }

        private void HandleInput()
        {
            string[] commandProperties = _input.Split(' ');
            string commandId = commandProperties[0];

            foreach (DebugCommandBase command in _commandList)
            {
                if (commandId == command.Id)
                {
                    if (command as DebugCommand != null)
                    {
                        (command as DebugCommand).Invoke();
                    }
                    if (command as DebugCommand<string> != null)
                    {
                        (command as DebugCommand<string>).Invoke(commandProperties[1]);
                    }

                    DebugManager.Instance.Log(LogLevel.Info, DebugCategory.Debug, $"Called debug command \"{command.Id}\". " +
                        $"Parameters: {(commandProperties.Length > 1 ? string.Join(", ", commandProperties.Skip(1)) : "None")}.");
                    return;
                }
            }

            DebugManager.Instance.Log(LogLevel.Info, DebugCategory.Debug, $"Tried to call unrecognized debug command \"{commandId}\".");
        }

        private void AddCommand(DebugCommandBase command)
        {
            _commandList.Add(command);
        }
    }
}
