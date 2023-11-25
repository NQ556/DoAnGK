#include "Header.h"

bool createVolume(const string& volumeName, uint64_t volumeSize, BootSector& bootSector) {
    // Kiểm tra volume tồn tại chưa
    ifstream checkFile(volumeName);
    if (checkFile.is_open()) 
    {
        checkFile.close();
        cout << "Volume '" << volumeName << "' đã tồn tại." << endl;
        return false;
    }

    string inputpass = "";
    string tmp = "";

    do 
    {
        cout << "Do you want set password of volume? (Yes/No)";
        cin >> inputpass;
        if (inputpass == "Yes") 
        {
            cout << "Password: ";
            cin >> tmp;
            bootSector.setPassSize(tmp.length());
            bootSector.setPassword(tmp);
        }
    } while (inputpass == "No");

    // Tạo volume rỗng với elements value 0
    ofstream volumeFile(volumeName, ios::out | ios::binary);
    if (volumeFile.is_open()) 
    {
        // Initialize with 0
        volumeFile.seekp(0, ios::beg);
        char emptyByte = 0;
        for (uint64_t i = 0; i < volumeSize; ++i) 
        {
            volumeFile.write(&emptyByte, sizeof(char));
        }

        cout << "Volume '" << volumeName << "' được tạo thành công." << endl;

        volumeFile.close();
        return true;
    }

    else
    {
        cout << "Không thể tạo volume." << endl;
        return false;
    }
}

void formatVolume(const string& volumeName, const BootSector& bootSector, 
    const FatTable& fatTable) {
    // Tạo và ghi BootSector vào volume
    ofstream volumeFile(volumeName, ios::in | ios::out | ios::binary);
    if (volumeFile.is_open()) 
    {
        volumeFile.seekp(0, ios::beg);
        volumeFile.write(reinterpret_cast<const char*>(&bootSector), sizeof(BootSector));

        // Tạo FatTable và ghi vào volume
        volumeFile.seekp(sizeof(BootSector), ios::beg);
        volumeFile.write(reinterpret_cast<const char*>(&fatTable.status[0]), fatTable.size * sizeof(uint16_t));

        // Cập nhật BootSector và FatTable trong volume
        volumeFile.seekp(0, ios::beg);
        volumeFile.write(reinterpret_cast<const char*>(&bootSector), sizeof(BootSector));

        cout << "Volume '" << volumeName << "' được định dạng thành công." << endl;

        volumeFile.close();
    }
    
    else
    {
        cout << "Không thể mở volume/ volume không tồn tại." << endl;
    }
}

void importFile(const string& volumeName, BootSector& bootSector,
    FatTable& fatTable, const string& fileName, const string& extension) {
    // Mở Volume để đọc và ghi
    fstream volumeFile(volumeName, ios::in | ios::out | ios::binary);
    if (!volumeFile.is_open()) 
    {
        cout << "Không thể mở volume/ volume không tồn tại." << endl;
        return;
    }

    // Mở file cần import
    ifstream sourceFile(fileName + "." + extension, ios::binary);
    if (!sourceFile.is_open()) 
    {
        cout << "Không thể mở file cần import/ file cần import không tồn tại." << endl;
        volumeFile.close();
        return;
    }

    // Đọc Fat table từ volume
    volumeFile.seekg(sizeof(BootSector), ios::beg);
    volumeFile.read(reinterpret_cast<char*>(&fatTable.status[0]), fatTable.size * sizeof(uint16_t));

    // Đọc root directory entry từ volume
    Entry rootDirectoryEntry;
    volumeFile.seekg(sizeof(BootSector) + fatTable.size * sizeof(uint16_t), ios::beg);
    volumeFile.read(reinterpret_cast<char*>(&rootDirectoryEntry), sizeof(Entry));

    // Kiểm tra xem file cần import có tồn tại trong root directory entry không
    for (uint16_t i = 0; i < rootDirectoryEntry.getNumEntryChild(); ++i) 
    {
        Entry existingEntry;

        if (i >= 1)
        {
            volumeFile.read(reinterpret_cast<char*>(&existingEntry), sizeof(Entry));
        }
        

        if (strcmp(existingEntry.MainName, fileName.c_str()) == 0)
        {
            std::cout << "File '" << fileName << "' đã tồn tại trong root directory." << std::endl;
            volumeFile.close();
            sourceFile.close();
            return;
        }
    }

    // Lấy size của file cần import
    sourceFile.seekg(0, ios::end);
    uint64_t fileSize = sourceFile.tellg();
    sourceFile.seekg(0, ios::beg);

    // Kiếm free cluster trong FAT table
    uint16_t freeCluster = 0;
    for (uint16_t i = 2; i < fatTable.size; ++i) 
    {
        if (fatTable.status[i] == 0) {
            freeCluster = i;
            // Đánh dấu cluster này đang được sử dụng
            fatTable.status[i] = 0xFFFF;  
            break;
        }
    }

    if (freeCluster == 0) {
        cout << "Không có cluster rỗng." << endl;
        volumeFile.close();
        sourceFile.close();
        return;
    }

    // Tạo entry mới cho file
    Entry newFileEntry;
    strcpy(newFileEntry.MainName, fileName.c_str());
    strcpy(newFileEntry.Extension, extension.c_str());
    newFileEntry.setFileSize(static_cast<uint32_t>(fileSize));
    newFileEntry.setStartCluster(freeCluster);

    int choice = 0;
    string tmp = "";

    if (!isListEmpty(volumeName, fatTable))
    {
        do
        {
            cout << "Bạn có muốn đặt mật khẩu cho file không?" << endl;
            cout << "1. Có." << endl;
            cout << "2. Không" << endl;
            cin >> choice;
            if (choice == 1)
            {
                cout << "Password: ";
                cin >> tmp;
                newFileEntry.setPassSize(tmp.length());
                newFileEntry.setPassword(tmp);
            }

            else if (choice == 2)
            {
                break;
            }
        } while (choice != 1 && choice != 2);
    }

    // Ghi entry mới vào volume
    volumeFile.seekp(sizeof(BootSector) + fatTable.size * sizeof(uint16_t) +
        rootDirectoryEntry.getNumEntryChild() * sizeof(Entry), ios::beg);
    volumeFile.write(reinterpret_cast<const char*>(&newFileEntry), sizeof(Entry));

    // Tăng số entry có trong root directory
    rootDirectoryEntry.setNumEntryChild(rootDirectoryEntry.getNumEntryChild() + 1);

    // Cập nhật root directory entry trong volume
    volumeFile.seekp(sizeof(BootSector) + fatTable.size * sizeof(uint16_t), ios::beg);
    volumeFile.write(reinterpret_cast<const char*>(&rootDirectoryEntry), sizeof(Entry));

    // Cập nhật FAT table trong volume
    volumeFile.seekp(sizeof(BootSector), ios::beg);
    volumeFile.write(reinterpret_cast<const char*>(&fatTable.status[0]), fatTable.size * sizeof(uint16_t));

    // Tính starting position để bắt đầu ghi nội dung file vào volume
    uint64_t fileContentStart = sizeof(BootSector) + fatTable.size * sizeof(uint16_t) +
        bootSector.getNumberOfEntry() * sizeof(Entry) +
        (freeCluster - 2) * bootSector.getNumSectorPerCluster() *
        bootSector.getnumBytePerSector();

    // Di chuyển đến starting position trong volume
    volumeFile.seekp(fileContentStart, ios::beg);

    // Bắt đầu ghi nội dung file
    const uint64_t bufferSize = bootSector.getnumBytePerSector() * bootSector.getNumSectorPerCluster();
    char buffer[4096];
    while (!sourceFile.eof()) 
    {
        sourceFile.read(buffer, bufferSize);
        volumeFile.write(buffer, sourceFile.gcount());
    }

    volumeFile.close();
    sourceFile.close();
}

void exportFile(const string& volumeName, BootSector& bootSector,
    const FatTable& fatTable, const string& fileName, const string& extension) {
    // Mở volume
    ifstream volumeFile(volumeName, ios::binary);
    if (!volumeFile.is_open()) 
    {
        cout << "Unable to open volume file for exporting file." << endl;
        return;
    }

    // Đọc root directory entry từ volume
    volumeFile.seekg(sizeof(BootSector) + fatTable.size * sizeof(uint16_t), ios::beg);
    Entry rootDirectoryEntry;
    volumeFile.read(reinterpret_cast<char*>(&rootDirectoryEntry), sizeof(Entry));

    // Kiếm file entry trong root directory
    bool fileFound = false;
    Entry fileEntry;
    for (uint16_t i = 0; i < rootDirectoryEntry.getNumEntryChild(); ++i) 
    {
        volumeFile.read(reinterpret_cast<char*>(&fileEntry), sizeof(Entry));

        // Kiểm tra xem có phải file cần export không
        if (strcmp(fileEntry.MainName, fileName.c_str()) == 0) {
            fileFound = true;
            break;
        }
    }

    if (!fileFound) {
        cout << "File '" << fileName << "' không có trong volume." << endl;
        volumeFile.close();
        return;
    }

    // Tạo một file mới để export
    ofstream outputFile(fileName + "_exported." + extension, ios::binary);
    if (!outputFile.is_open()) 
    {
        cout << "Không thể tạo file mới." << endl;
        volumeFile.close();
        return;
    }

    // Đọc và ghi nội dung file vào file vừa tạo
    uint16_t currentCluster = fileEntry.getStartCluster();
    uint64_t remainingBytes = fileEntry.getFileSize();

    while (remainingBytes > 0) 
    {
        // Lấy vị trí bắt đầu ghi nội dung file trong volume
        uint64_t clusterPosition = sizeof(BootSector) + fatTable.size * sizeof(uint16_t) +
            bootSector.getNumberOfEntry() * sizeof(Entry) +
            (currentCluster - 2) * bootSector.getNumSectorPerCluster() *
            bootSector.getnumBytePerSector();

        // Di chuyển đến vị trí bắt đầu ghi nội dung
        volumeFile.seekg(clusterPosition, ios::beg);

        // Tính số bytes cần đọc từ current cluster
        uint64_t tmp = static_cast<uint64_t>(bootSector.getNumSectorPerCluster() * bootSector.getnumBytePerSector());
        uint64_t bytesRead = remainingBytes;

        if (tmp < bytesRead) 
        {
            bytesRead = tmp;
        }

        // Đọc và ghi nội dung của current cluster
        std::vector<char> buffer(bytesRead);
        volumeFile.read(buffer.data(), bytesRead);
        outputFile.write(buffer.data(), bytesRead);

        // Cập nhật remaining bytes và di chuyển tới cluster tiếp theo
        remainingBytes -= bytesRead;
        currentCluster = fatTable.status[currentCluster];
    }
    
    volumeFile.close();
    outputFile.close();

    cout << "File '" << fileName << "' được xuất thành công." << endl;
}

bool isListEmpty(const string& volumeName, const FatTable& fatTable) {
    // Mở Volume
    ifstream volumeFile(volumeName, ios::binary);

    if (!volumeFile.is_open())
    {
        return true;
    }

    // Đọc root directory entry từ volume
    volumeFile.seekg(sizeof(BootSector) + fatTable.size * sizeof(uint16_t), ios::beg);
    Entry rootDirectoryEntry = Entry();
    volumeFile.read(reinterpret_cast<char*>(&rootDirectoryEntry), sizeof(Entry));

    // Kiểm tra xem root directory có rỗng không
    if (rootDirectoryEntry.getNumEntryChild() == 0)
    {
        volumeFile.close();
        return true;
    }

    return false;
}

void listFiles(const string& volumeName, const BootSector& bootSector, const FatTable& fatTable) {
    // Mở volume
    ifstream volumeFile(volumeName, ios::binary);
    if (!volumeFile.is_open()) 
    {
        cout << "Không mở được volume/ volume không tồn tại." << endl;
        return;
    }

    // Đọc root directory entry from the volume
    volumeFile.seekg(sizeof(BootSector) + fatTable.size * sizeof(uint16_t), ios::beg);
    Entry rootDirectoryEntry = Entry();
    volumeFile.read(reinterpret_cast<char*>(&rootDirectoryEntry), sizeof(Entry));

    // Kiểm tra xem root directory có rỗng không
    if (rootDirectoryEntry.getNumEntryChild() == 0) 
    {
        cout << "Không có file trong root directory." << endl;
        volumeFile.close();
        return;
    }

    // Loop qua từng file trong root directory
    cout << "Danh sách các file trong root directory:" << endl;
    for (uint16_t i = 1; i < rootDirectoryEntry.getNumEntryChild(); ++i) 
    {
        Entry fileEntry;
        try 
        {
            volumeFile.read(reinterpret_cast<char*>(&fileEntry), sizeof(Entry));
            
        }
        catch (const std::exception& e) 
        {
            // Error
        }

        if (fileEntry.MainName[0] != '\0')
        {
            cout << "File Entry: Tên = " << fileEntry.MainName << "." << fileEntry.Extension
                << ", Kích thước = " << fileEntry.getFileSize() << " bytes" << endl;
            return;
            
        }  
    }

    volumeFile.close();
}