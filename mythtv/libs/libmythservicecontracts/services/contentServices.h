//////////////////////////////////////////////////////////////////////////////
// Program Name: contentServices.h
// Created     : Mar. 7, 2011
//
// Purpose - Content Services API Interface definition 
//
// Copyright (c) 2010 David Blain <dblain@mythtv.org>
//                                          
// This library is free software; you can redistribute it and/or 
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or at your option any later version of the LGPL.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library.  If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef CONTENTSERVICES_H_
#define CONTENTSERVICES_H_

#include <QFileInfo>
#include <QStringList>

#include "service.h"
#include "datacontracts/artworkInfoList.h"
#include "datacontracts/liveStreamInfoList.h"

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Notes -
//
//  * This implementation can't handle declared default parameters
//
//  * When called, any missing params are sent default values for its datatype
//
//  * Q_CLASSINFO( "<methodName>_Method", ...) is used to determine HTTP method
//    type.  Defaults to "BOTH", available values:
//          "GET", "POST" or "BOTH"
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class SERVICE_PUBLIC ContentServices : public Service  //, public QScriptable ???
{
    Q_OBJECT
    Q_CLASSINFO( "version"    , "1.32" );
    Q_CLASSINFO( "DownloadFile_Method",            "POST" )

    public:

        // Must call InitializeCustomTypes for each unique Custom Type used
        // in public slots below.

        ContentServices( QObject *parent = 0 ) : Service( parent )
        {
            DTC::ArtworkInfoList::InitializeCustomTypes();
            DTC::LiveStreamInfoList::InitializeCustomTypes();
        }

    public slots:

        virtual QFileInfo           GetFile             ( const QString   &StorageGroup,
                                                          const QString   &FileName ) = 0;

        virtual QFileInfo           GetImageFile        ( const QString   &StorageGroup,
                                                          const QString   &FileName,
                                                          int Width, int Height ) = 0;

        virtual QStringList         GetFileList         ( const QString   &StorageGroup ) = 0;

        virtual QFileInfo           GetRecordingArtwork ( const QString   &Type,
                                                          const QString   &Inetref,
                                                          int Season, int Width,
                                                          int Height ) = 0;

        virtual DTC::ArtworkInfoList*
                                    GetRecordingArtworkList( int              ChanId,
                                                             const QDateTime &StartTime  ) = 0;

        virtual DTC::ArtworkInfoList*
                                    GetProgramArtworkList( const QString &Inetref,
                                                           int            Season  ) = 0;


        virtual QFileInfo           GetVideoArtwork     ( const QString   &Type,
                                                          int Id, int Width,
                                                          int Height ) = 0;

        virtual QFileInfo           GetAlbumArt         ( int Id, int Width, int Height ) = 0;

        virtual QFileInfo           GetPreviewImage     ( int              ChanId,
                                                          const QDateTime &StartTime,
                                                          int              Width,    
                                                          int              Height,   
                                                          int              SecsIn ) = 0;

        virtual QFileInfo           GetRecording        ( int              ChanId,
                                                          const QDateTime &StartTime ) = 0;

        virtual QFileInfo           GetMusic            ( int Id ) = 0;
        virtual QFileInfo           GetVideo            ( int Id ) = 0;

        virtual QString             GetHash             ( const QString   &StorageGroup,
                                                          const QString   &FileName ) = 0;

        virtual bool                DownloadFile        ( const QString   &URL,
                                                          const QString   &StorageGroup ) = 0;

        virtual DTC::LiveStreamInfo     *AddLiveStream          ( const QString   &StorageGroup,
                                                                  const QString   &FileName,
                                                                  const QString   &HostName,
                                                                  const QString   &MaxSegments,
                                                                  const QString   &Width,
                                                                  const QString   &Height,
                                                                  const QString   &Bitrate,
                                                                  const QString   &AudioBitrate,
                                                                  const QString   &SampleRate ) = 0;

        virtual DTC::LiveStreamInfo     *AddRecordingLiveStream ( int              ChanId,
                                                                  const QDateTime &StartTime,
                                                                  const QString   &MaxSegments,
                                                                  const QString   &Width,
                                                                  const QString   &Height,
                                                                  const QString   &Bitrate,
                                                                  const QString   &AudioBitrate,
                                                                  const QString   &SampleRate ) = 0;

        virtual DTC::LiveStreamInfo     *AddVideoLiveStream     ( int              Id,
                                                                  const QString   &MaxSegments,
                                                                  const QString   &Width,
                                                                  const QString   &Height,
                                                                  const QString   &Bitrate,
                                                                  const QString   &AudioBitrate,
                                                                  const QString   &SampleRate ) = 0;

        virtual DTC::LiveStreamInfo     *GetLiveStream            ( int Id ) = 0;
        virtual DTC::LiveStreamInfoList *GetLiveStreamList        ( void ) = 0;
        virtual DTC::LiveStreamInfoList *GetFilteredLiveStreamList( const QString &FileName ) = 0;

        virtual DTC::LiveStreamInfo     *StopLiveStream         ( int Id ) = 0;
        virtual bool                     RemoveLiveStream       ( int Id ) = 0;
};

#endif

