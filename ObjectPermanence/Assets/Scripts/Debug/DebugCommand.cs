using System;

namespace ObjectPermanence
{
    /**
     * The wrapper for debug commands.
     * 
     * Notes:
     * - Ngl it seems a little bot that we have to create a derived DebugCommand rather than having a vardic template
     *   or the like, but it seems C# doesn't support this w/o some sort of code generation.
     * - If a new number of parameters for a debug command is required, just create a new class with the required
     *   number of template arguements.
     */
    public abstract class DebugCommandBase
    {
        public string Id { get; private set; }
        public string Desc { get; private set; }
        public string Format { get; private set; }

        public DebugCommandBase(string id, string desc, string format)
        {
            Id = id;
            Desc = desc;
            Format = format;
        }
    }


    public sealed class DebugCommand : DebugCommandBase
    {
        private Action _command;

        public DebugCommand(string id, string desc, string format, Action command) : base(id, desc, format)
        {
            _command = command;
        }

        public void Invoke()
        {
            _command.Invoke();
        }
    }

    public sealed class DebugCommand<Arg1> : DebugCommandBase
    {
        private Action<Arg1> _command;

        public DebugCommand(string id, string desc, string format, Action<Arg1> command) : base(id, desc, format)
        {
            _command = command;
        }

        public void Invoke(Arg1 arg1)
        {
            _command.Invoke(arg1);
        }
    }
}
