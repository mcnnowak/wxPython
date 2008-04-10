/////////////////////////////////////////////////////////////////////////////
// Name:        protocol/ftp.h
// Purpose:     interface of wxFTP
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    Transfer modes used by wxFTP.
*/
enum TransferMode
{
    NONE,       //!< not set by user explicitly.
    ASCII,
    BINARY
};

/**
    @class wxFTP
    @headerfile ftp.h wx/protocol/ftp.h

    wxFTP can be used to establish a connection to an FTP server and perform all the
    usual operations. Please consult the RFC 959 for more details about the FTP
    protocol.

    To use a command which doesn't involve file transfer (i.e. directory oriented
    commands) you just need to call a corresponding member function or use the
    generic wxFTP::SendCommand() method.
    However to actually transfer files you just get or give a stream to or from this
    class and the actual data are read or written using the usual stream methods.

    Example of using wxFTP for file downloading:

    @code
        wxFTP ftp;

        // if you don't use these lines anonymous login will be used
        ftp.SetUser("user");
        ftp.SetPassword("password");

        if ( !ftp.Connect("ftp.wxwindows.org") )
        {
            wxLogError("Couldn't connect");
            return;
        }

        ftp.ChDir("/pub");
        wxInputStream *in = ftp.GetInputStream("wxWidgets-4.2.0.tar.gz");
        if ( !in )
        {
            wxLogError("Coudln't get file");
        }
        else
        {
            size_t size = in-GetSize();
            char *data = new char[size];
            if ( !in->Read(data, size) )
            {
                wxLogError("Read error");
            }
            else
            {
                // file data is in the buffer
                ...
            }

            delete [] data;
            delete in;
        }
    @endcode

    To upload a file you would do (assuming the connection to the server was opened
    successfully):

    @code
        wxOutputStream *out = ftp.GetOutputStream("filename");
        if ( out )
        {
            out->Write(...); // your data
            delete out;
        }
    @endcode

    @library{wxnet}
    @category{net}

    @see wxSocketBase
*/
class wxFTP : public wxProtocol
{
public:
    /**
        Default constructor.
    */
    wxFTP();

    /**
        Destructor will close the connection if connected.
    */
    ~wxFTP();

    /**
        Aborts the download currently in process, returns @true if ok, @false
        if an error occurred.
    */
    bool Abort();

    /**
        Change the current FTP working directory.
        Returns @true if successful.
    */
    bool ChDir(const wxString& dir);

    /**
        Send the specified @a command to the FTP server. @a ret specifies
        the expected result.

        @returns @true if the command has been sent successfully, else @false.
    */
    bool CheckCommand(const wxString& command, char ret);

    /**
        Returns @true if the given remote file exists, @false otherwise.
    */
    bool FileExists(const wxString& filename);

    /**
        The GetList() function is quite low-level. It returns the list of the files in
        the current directory. The list can be filtered using the @a wildcard string.

        If @a wildcard is empty (default), it will return all files in directory.
        The form of the list can change from one peer system to another. For example,
        for a UNIX peer system, it will look like this:

        @verbatim
        -r--r--r--   1 guilhem  lavaux      12738 Jan 16 20:17 cmndata.cpp
        -r--r--r--   1 guilhem  lavaux      10866 Jan 24 16:41 config.cpp
        -rw-rw-rw-   1 guilhem  lavaux      29967 Dec 21 19:17 cwlex_yy.c
        -rw-rw-rw-   1 guilhem  lavaux      14342 Jan 22 19:51 cwy_tab.c
        -r--r--r--   1 guilhem  lavaux      13890 Jan 29 19:18 date.cpp
        -r--r--r--   1 guilhem  lavaux       3989 Feb  8 19:18 datstrm.cpp
        @endverbatim

        But on Windows system, it will look like this:

        @verbatim
        winamp~1 exe    520196 02-25-1999  19:28  winamp204.exe
            1 file(s)           520 196 bytes
        @endverbatim

        @return @true if the file list was successfully retrieved, @false otherwise.

        @see GetFilesList()
    */
    bool GetDirList(wxArrayString& files,
                    const wxString& wildcard = "");

    /**
        Returns the file size in bytes or -1 if the file doesn't exist or the size
        couldn't be determined.

        Notice that this size can be approximative size only and shouldn't be used
        for allocating the buffer in which the remote file is copied, for example.
    */
    int GetFileSize(const wxString& filename);

    /**
        This function returns the computer-parsable list of the files in the current
        directory (optionally only of the files matching the @e wildcard, all files
        by default).

        This list always has the same format and contains one full (including the
        directory path) file name per line.

        @returns @true if the file list was successfully retrieved, @false otherwise.

        @see GetDirList()
    */
    bool GetFilesList(wxArrayString& files,
                      const wxString& wildcard = "");

    /**
        Creates a new input stream on the specified path.

        You can use all but the seek functionality of wxStreamBase.
        wxStreamBase::Seek() isn't available on all streams. For example, HTTP or FTP
        streams do not deal with it. Other functions like wxStreamBase::Tell() are
        not available for this sort of stream, at present.

        You will be notified when the EOF is reached by an error.

        @returns Returns @NULL if an error occurred (it could be a network failure
                 or the fact that the file doesn't exist).
    */
    wxInputStream* GetInputStream(const wxString& path);

    /**
        Returns the last command result, i.e. the full server reply for the last command.
    */
    const wxString GetLastResult();

    /**
        Initializes an output stream to the specified @e file.

        The returned stream has all but the seek functionality of wxStreams.
        When the user finishes writing data, he has to delete the stream to close it.

        @returns An initialized write-only stream.

        @see wxOutputStream
    */
    wxOutputStream* GetOutputStream(const wxString& file);

    /**
        Create the specified directory in the current FTP working directory.
        Returns @true if successful.
    */
    bool MkDir(const wxString& dir);

    /**
        Returns the current FTP working directory.
    */
    wxString Pwd();

    /**
        Rename the specified @a src element to @e dst. Returns @true if successful.
    */
    bool Rename(const wxString& src, const wxString& dst);

    /**
        Remove the specified directory from the current FTP working directory.
        Returns @true if successful.
    */
    bool RmDir(const wxString& dir);

    /**
        Delete the file specified by @e path. Returns @true if successful.
    */
    bool RmFile(const wxString& path);

    /**
        Send the specified @a command to the FTP server and return the first
        character of the return code.
    */
    char SendCommand(const wxString& command);

    /**
        Sets the transfer mode to ASCII. It will be used for the next transfer.
    */
    bool SetAscii();

    /**
        Sets the transfer mode to binary (IMAGE). It will be used for the next transfer.
    */
    bool SetBinary();

    /**
        If @a pasv is @true, passive connection to the FTP server is used.

        This is the default as it works with practically all firewalls.
        If the server doesn't support passive move, you may call this function with
        @false argument to use active connection.
    */
    void SetPassive(bool pasv);

    /**
        Sets the password to be sent to the FTP server to be allowed to log in.
    */
    void SetPassword(const wxString& passwd);

    /**
        Sets the transfer mode to the specified one. It will be used for the next
        transfer.

        If this function is never called, binary transfer mode is used by default.
    */
    bool SetTransferMode(TransferMode mode);

    /**
        Sets the user name to be sent to the FTP server to be allowed to log in.
    */
    void SetUser(const wxString& user);
};

