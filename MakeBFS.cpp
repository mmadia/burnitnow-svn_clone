#include "MakeBFS.h"

extern char *IMAGE_NAME;
extern char *BURN_DIR;
extern char VOL_NAME[25];

BDirectory *BFSDir;
off_t total_size;


int copy_data(const entry_ref& source, const entry_ref& dest)
{
	status_t err;
	unsigned char *buffer;
	buffer = new unsigned char[BUFFERLEN];	
	ssize_t nRead;	

	BFile srcFile(&source, O_RDONLY);
	if ((err = srcFile.InitCheck()) != B_NO_ERROR) return err;

	BFile destFile(&dest, O_WRONLY | O_CREAT | O_TRUNC);
	if ((err = destFile.InitCheck()) != B_NO_ERROR) return err;

	struct stat info;
	if ((err = srcFile.GetStat(&info)) != B_NO_ERROR) return err;
	if ((err = destFile.SetSize(info.st_size)) != B_NO_ERROR) return err;
	if ((err = destFile.Seek(0, SEEK_SET)) != B_NO_ERROR) return err;

	while ((nRead = srcFile.Read(buffer, BUFFERLEN)) > 0)
	{
		err = destFile.Write(buffer, nRead);
		if (err < 0) return err;
	}
	delete buffer;
}
int copy_attributes(const entry_ref& source, const entry_ref& dest)
{
	status_t err;
	unsigned char *buffer;
	buffer = new unsigned char[BUFFERLEN];	

	BNode srcNode(&source);
	if ((err = srcNode.InitCheck()) != B_NO_ERROR) return err;

	BNode destNode(&dest);
	if ((err = destNode.InitCheck()) != B_NO_ERROR) return err;


	char attrName[B_ATTR_NAME_LENGTH];
	srcNode.RewindAttrs();
	while ((err = srcNode.GetNextAttrName(attrName)) == B_NO_ERROR)
	{
		attr_info info;

		if ((err = srcNode.GetAttrInfo(attrName, &info)) != B_NO_ERROR) return err;

		ssize_t nRead;
		off_t offset = 0;
		while ((offset < info.size) && ((nRead = srcNode.ReadAttr(attrName, info.type, offset, buffer, BUFFERLEN)) > 0))
		{
			err = destNode.WriteAttr(attrName, info.type, offset, buffer, nRead);
			if (err < 0)
			{
				return err;
			}

			offset += nRead;
		}
	}


	delete buffer;
}

status_t CopyFile(const entry_ref &source, const entry_ref &dest)
{
	copy_data(source,dest);
	copy_attributes(source,dest);
	return 0;
}


void GetSizeLoop(entry_ref *ref)
{
	struct stat temp;
	entry_ref temp_ref,t_ref;
	BPath temp_path;
	BDirectory *temp_dir;
	BEntry temp_entry;
	temp_dir = new BDirectory(ref);

	while(temp_dir->GetNextRef(&temp_ref) == B_OK) {
		if(BEntry(&temp_ref,true).IsDirectory()) {
				GetSizeLoop(&temp_ref);
				total_size += 1024;

		}
		else {
			if(temp_dir->GetEntry(&temp_entry) == B_OK) 
				if(temp_entry.GetPath(&temp_path) == B_OK) {
					BEntry(&temp_ref,true).GetStat(&temp);
					total_size += temp.st_size + 1024;

				}
		}
	}
	delete temp_dir;
}

uint64 GetBFSSize() {
	entry_ref temp_ref;
	BEntry(BURN_DIR,true).GetRef(&temp_ref);
	total_size = 0;
	GetSizeLoop(&temp_ref);
	total_size += 10*1024*1024;
	return (uint64)total_size;
} 
int MakeBFS() {
	FILE *f1;
	char temp[1024],buffer[1024];
	sprintf(temp,"dd if=/dev/zero of=\"%s\" bs=1024k count=%lld",IMAGE_NAME,(GetBFSSize()/1024/1024));
	f1 = popen(temp,"r");
	while (!feof(f1) && !ferror(f1))
	{
		fgets(buffer,1024,f1);
	}
	pclose(f1);
	sprintf(temp,"sync; mkbfs 2048 \"%s\" \"%s\"",IMAGE_NAME,VOL_NAME);
	f1 = popen(temp,"r");
	while (!feof(f1) && !ferror(f1))
	{
		fgets(buffer,1024,f1);
	}
	pclose(f1);
}
int UnmountBFS() {
	char temp[1024];
	sprintf(temp,"sync; unmount /BurnITNOWTempMount");
	system(temp);
	sprintf(temp,"rmdir /BurnITNOWTempMount");
	system(temp);
	return 0;
}

int MountBFS() {
	FILE *f1;
	entry_ref temp_ref;
	BDirectory *temp_dir;
	char command[1024],buffer[1024];
	
	temp_dir = new BDirectory("/");
	temp_dir->CreateDirectory("/BurnITNOWTempMount",NULL);
	
	sprintf(command,"sync ; mount \"%s\" /BurnITNOWTempMount",IMAGE_NAME);
	f1 = popen(command,"r");
	while (!feof(f1) && !ferror(f1))
	{
		fgets(buffer,1024,f1);
	}
	pclose(f1);
	delete temp_dir;
}

void Copy_Loop(entry_ref *ref) {
	entry_ref temp_ref,t_ref,t_ref2;
	status_t err;
	char temp_char[4096];
	BPath temp_path;
	BDirectory *temp_dir;
	BEntry temp_entry;
	temp_dir = new BDirectory(ref);
	while(temp_dir->GetNextRef(&temp_ref) == B_OK)
	{
		if(BEntry(&temp_ref,true).IsDirectory())
		{
			BFSDir->GetEntry(&temp_entry);
			temp_entry.GetPath(&temp_path);
			memset(temp_char,0,sizeof(temp_char));
			sprintf(temp_char,"%s/%s",temp_path.Path(),temp_ref.name);
			if((err = BFSDir->CreateDirectory(temp_char,NULL)) == B_OK)
			{
				if(BFSDir->SetTo(temp_char) == B_OK)
				{
					Copy_Loop(&temp_ref);
					BFSDir->SetTo(temp_path.Path());
				}	
			}
		}
		else {
			if(BFSDir->GetEntry(&temp_entry) == B_OK) 
				if(temp_entry.GetPath(&temp_path) == B_OK) {
					sprintf(temp_char,"%s/%s",temp_path.Path(),temp_ref.name);
					BEntry(&temp_ref,true).GetRef(&t_ref2);
					if(BEntry(temp_char,true).GetRef(&t_ref) == B_OK) {
							CopyFile(t_ref2,t_ref);
					}
				}
		}

	}
	delete temp_dir;
}

int CopyFiles() {
	entry_ref temp_ref;
	BEntry(BURN_DIR).GetRef(&temp_ref);
	Copy_Loop(&temp_ref);
	return 0;
}
