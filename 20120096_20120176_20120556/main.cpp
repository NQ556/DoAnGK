#include "Header.h"

int main()
{
    string volumeName = "";
    string fileName = "";
    string fileExtension = "";
    uint64_t volumeSize = 1024 * 1024; 
    uint64_t inputSize = 0;
    //10MB
    int choice = 0;

    BootSector bootSector = BootSector(volumeSize);
    FatTable fatTable = FatTable(bootSector.getFatSize());
    bool exceptionThrown = false;

    do
    {
        cout << "1. Tạo và định dạng Volume." << endl;
        cout << "2. Liệt kê danh sách các file trong volume." << endl;
        cout << "3. Chép 1 file từ bên ngoài vào volume." << endl;
        cout << "4. Chép 1 file từ trong volume ra ngoài." << endl;
        cout << "0. Thoát." << endl;

        cout << "Lựa chọn: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "Nhập tên volume: (VD: test.bin)" << endl;
            cin >> volumeName;

            cout << "Nhập kích thước volume (MB): (VD: 10)" << endl;
            cin >> inputSize;

            if (createVolume(volumeName, volumeSize * inputSize, bootSector))
            {
                formatVolume(volumeName, bootSector, fatTable);

            }
            break;

        case 2:
            cout << "Nhập tên volume: (VD: test.bin)" << endl;
            cin >> volumeName;
            listFiles(volumeName, bootSector, fatTable);
            break;
        case 3:
            cout << "Nhập tên volume: (VD: test.bin)" << endl;
            cin >> volumeName;
            cout << "Nhập tên file cần chép: (VD: test)" << endl;
            cin >> fileName;
            cout << "Nhập extension của file cần chép: (VD: txt)" << endl;
            cin >> fileExtension;

            if (isListEmpty(volumeName, fatTable))
            {
                importFile(volumeName, bootSector, fatTable, fileName, fileExtension);
                importFile(volumeName, bootSector, fatTable, fileName, fileExtension);
            }

            else
            {
                importFile(volumeName, bootSector, fatTable, fileName, fileExtension);
            }
            break;

        case 4:
            cout << "Nhập tên volume: (VD: test.bin)" << endl;
            cin >> volumeName;
            cout << "Nhập tên file cần chép: (VD: test)" << endl;
            cin >> fileName;
            cout << "Nhập extension của file cần chép: (VD: txt)" << endl;
            cin >> fileExtension;

            exportFile(volumeName, bootSector, fatTable, fileName, fileExtension);
            break;

        default:
            break;
        }

    } while (choice != 0);

    //listFiles(volumeName, bootSector, fatTable);

    
    /*if (isListEmpty(volumeName, fatTable))
    {
        importFile(volumeName, bootSector, fatTable, "hello", "txt");
        importFile(volumeName, bootSector, fatTable, "hello", "txt");
    }

    else
    {
        importFile(volumeName, bootSector, fatTable, "hello", "txt");
    }*/

    //importFile(volumeName, bootSector, fatTable, "test", "txt");
    //exportFile(volumeName, bootSector, fatTable, "hello", "txt");

    return 0;
}