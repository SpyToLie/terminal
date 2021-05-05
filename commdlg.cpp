HRESULT BasicFileOpen()
{
    // CoCreate the File Open Dialog object.
    IFileDialog* pfd = NULL;
    RETURN_IF_FAILED(CoCreateInstance(CLSID_FileOpenDialog,
                                      NULL,
                                      CLSCTX_INPROC_SERVER,
                                      IID_PPV_ARGS(&pfd)));
    // Create an event handling object, and hook it up to the dialog.
    IFileDialogEvents* pfde = NULL;
    RETURN_IF_FAILED(CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&pfde)));
    // Hook up the event handler.
    DWORD dwCookie;
    RETURN_IF_FAILED(pfd->Advise(pfde, &dwCookie));
    // Set the options on the dialog.
    DWORD dwFlags;

    // Before setting, always get the options first in order
    // not to override existing options.
    RETURN_IF_FAILED(pfd->GetOptions(&dwFlags));
    // In this case, get shell items only for file system items.
    RETURN_IF_FAILED(pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM));
    // Set the file types to display only.
    // Notice that this is a 1-based array.
    RETURN_IF_FAILED(pfd->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes));
    // Set the selected file type index to Word Docs for this example.
    RETURN_IF_FAILED(pfd->SetFileTypeIndex(INDEX_WORDDOC));
    // Set the default extension to be ".doc" file.
    RETURN_IF_FAILED(pfd->SetDefaultExtension(L"doc;docx"));
    // Show the dialog
    RETURN_IF_FAILED(pfd->Show(NULL));
    // Obtain the result once the user clicks
    // the 'Open' button.
    // The result is an IShellItem object.
    IShellItem* psiResult;
    RETURN_IF_FAILED(pfd->GetResult(&psiResult));
    // We are just going to print out the
    // name of the file for sample sake.
    PWSTR pszFilePath = NULL;
    RETURN_IF_FAILED(psiResult->GetDisplayName(SIGDN_FILESYSPATH,
                                               &pszFilePath));
    TaskDialog(NULL,
               NULL,
               L"CommonFileDialogApp",
               pszFilePath,
               NULL,
               TDCBF_OK_BUTTON,
               TD_INFORMATION_ICON,
               NULL);
    CoTaskMemFree(pszFilePath);
    psiResult->Release();
    // Unhook the event handler.
    pfd->Unadvise(dwCookie);
    pfde->Release();
    pfd->Release();
    return hr;
}
