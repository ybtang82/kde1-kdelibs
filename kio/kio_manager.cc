#include "kio_manager.h"

#include <k2config.h>

#include <string.h>
#include <assert.h>

#include <kapp.h>

ProtocolManager* ProtocolManager::s_pManager = 0L;

ProtocolManager::ProtocolManager()
{
  assert( !s_pManager );
  
  s_pManager = this;
  m_pConfig = 0L;
}

void ProtocolManager::init()
{
  if ( m_pConfig )
    return;
  
  string file = KApplication::kde_datadir().data();
  file += "/kio/kio.kfg";
  
  m_pConfig = new K2Config( file.c_str() );
}

bool ProtocolManager::supportsListing( const char *_protocol )
{
  list<string> lst;
  return ( listing( _protocol, lst ) );
}

bool ProtocolManager::listing( const char *_protocol, list<string>& _listing )
{
  K2Config *g = findIntern( _protocol );
  if ( !g )
  {
    cerr << "Protocol " << _protocol << " not found" << endl;
    return false;
  }
  
  return g->readStringList( "listing", _listing );
}

string ProtocolManager::find( const char *_protocol )
{
  K2Config *g = findIntern( _protocol );

  string exec;
  if ( g != 0L && g->readString( "exec", exec ) )
  {
    // We dont have KApplication here
    string file = KApplication::kde_bindir().data();
    file += "/";
    file += exec;
    return file;
  }
  
  return exec;
}

K2Config* ProtocolManager::findIntern( const char *_protocol )
{
  assert( s_pManager );
  
  init();

  K2Config::iterator it = m_pConfig->begin();
  for( ; it != m_pConfig->end(); it++ )
  {
    K2Config *g;
    if ( ( g = it.group() ) && strcmp( g->name(), _protocol ) == 0L )
      return g;
  }
  
  return 0L;
}

ProtocolManager::Type ProtocolManager::inputType( const char *_protocol )
{
  K2Config *g = findIntern( _protocol );
  if ( !g )
    return T_ERROR;
  
  string exec;
  if ( g != 0L && g->readString( "input", exec ) )
  {
    if ( exec == "stream" )
      return T_STREAM;
    else if ( exec == "filesystem" )
      return T_FILESYSTEM;
    else
      return T_NONE;
  }
  
  return T_NONE;
}

ProtocolManager::Type ProtocolManager::outputType( const char *_protocol )
{
  K2Config *g = findIntern( _protocol );
  if ( !g )
    return T_ERROR;
  
  string exec;
  if ( g != 0L && g->readString( "output", exec ) )
  {
    if ( exec == "stream" )
      return T_STREAM;
    else if ( exec == "filesystem" )
      return T_FILESYSTEM;
    else
      return T_NONE;
  }
  
  return T_NONE;
}
