using System.Collections.Generic;
using UnityEngine;

namespace ObjectPermanence
{
    // Declaring these outside of DebugManager breaks encapsulation a bit but means the calls to Assert/Log
    // won't be so long (eg: DebugManager.Instance.Assert([condtion], DebugManager.AssertLevel.[etc...])
    public enum AssertLevel : int
    {
        Soft, Assert, Fatal
    }
    public enum LogLevel : int
    {
        Info, Warning, Error
    };

    /**
     * All log messages and asserts are routed through this class so they can be actioned all in one place
     * This class is created on startup via the Bootstrapper. Assert and log levels can be toggled
     * through the inspector. See the Managers->DebugManager gameobject.
     * 
     * The syntax for asserting/logging is, for example:
     *  - DebugManager.Instance.Assert([condition], AssertLevel.Soft, DebugCategory.Debug, "Assert");
     *  - DebugManager.Instance.Log(LogLevel.Info, DebugCategory.Debug, "Log");
     */
    public sealed class DebugManager : Singleton<DebugManager>
    {
        [SerializeField] private AssertLevel _assertLevel;
        [SerializeField] private LogLevel _logLevel;
        [SerializeField] private Optional<DebugCategory> _categoryFilter;

        private readonly Dictionary<object /*AssertLevel or LogLevel*/, Color> _debugColours;

        private DebugManager()
        {
            _assertLevel = AssertLevel.Soft;
            _logLevel = LogLevel.Info;

            _debugColours = new Dictionary<object, Color>
            {
                { AssertLevel.Soft,   Color.gray   },
                { AssertLevel.Assert, Color.yellow },
                { AssertLevel.Fatal,  Color.red    },

                { LogLevel.Info,      Color.gray   },
                { LogLevel.Warning,   Color.yellow },
                { LogLevel.Error,     Color.red    }
            };
        }

        public void Assert(bool condition, AssertLevel level, DebugCategory category, string message)
        {
#if DEBUG
            if (!condition)
            {
                if (IsLevelAndCategoryValid((int)_assertLevel, ((int)level), _categoryFilter, category))
                {
                    string assertString = FormatMessageDebugMessage(level, category, message);
                    Debug.Assert(false, assertString);
                }
            }
#endif // if DEBUG
        }

        public void AssertFail(AssertLevel level, DebugCategory category, string message)
        {
            const bool condition = false;
            Assert(condition, level, category, message);
        }

        public void Log(LogLevel level, DebugCategory category, string message)
        {
#if DEBUG
            if (IsLevelAndCategoryValid((int)_logLevel, ((int)level), _categoryFilter, category))
            {
                string logString = FormatMessageDebugMessage(level, category, message);
                Debug.Log(logString);
            }
#endif // if DEBUG
        }

        public void ConditionalLog(bool condition, LogLevel level, DebugCategory category, string message)
        {
            if (condition)
            {
                Log(level, category, message);
            }
        }

        public AssertLevel GetAssertLevel()
        {
            return _assertLevel;
        }
        public void SetAssertLevel(AssertLevel assertLevel)
        {
            _assertLevel = assertLevel;
        }

        public LogLevel GetLogLevel()
        {
            return _logLevel;
        }
        public void SetLogLevel(LogLevel logLevel)
        {
            _logLevel = logLevel;
        }

        private bool IsLevelAndCategoryValid(int setLevel, int comparisonLevel,
            Optional<DebugCategory> setCategory, DebugCategory comparisonCategory)
        {
            // We could use dynamics here to keep the type, but that seems a little overkill :P
            return setLevel <= comparisonLevel && (!setCategory.Enabled || setCategory.Value == comparisonCategory);
        }

        private string FormatMessageDebugMessage<T>(T level, DebugCategory category, string message)
        {
            // I don't think I can use the 'where' keyword for this..?
            Assert(level is AssertLevel || level is LogLevel,
                AssertLevel.Assert, DebugCategory.Debug, "level must be AssertLevel or LogLevel");

            string levelColoured = StringUtils.ColourString(level.ToString(), _debugColours[level]);
            string messageString = $"[{levelColoured}] [{category}] {message}";
            return messageString;
        }
    }
}
