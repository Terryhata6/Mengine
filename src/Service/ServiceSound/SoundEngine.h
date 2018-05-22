#pragma once

#include "Interface/StreamInterface.h"
#include "Interface/SoundSystemInterface.h"

#include "Core/ServiceBase.h"

#include "SoundIdentity.h"
#include "ThreadWorkerSoundBufferUpdate.h"

#include "Core/ConstString.h"
#include "Factory/Factory.h"

#include "Config/Vector.h"

#include "math/vec3.h"

#include "stdex/stl_map.h"
#include "stdex/stl_vector.h"
#include "stdex/pool.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
	class SoundEngine
		: public ServiceBase<SoundServiceInterface>
	{
	public:
		SoundEngine();
		~SoundEngine() override;

    public:
		bool _initializeService() override;
		void _finalizeService() override;

    public:
        void _stopService() override;

	public:
		bool supportStreamSound() const override;

	public:
		void tick( float _current, float _timing ) override;

	public:
		void addSoundVolumeProvider( const SoundVolumeProviderInterfacePtr & _soundVolumeProvider ) override;
		bool removeSoundVolumeProvider(const SoundVolumeProviderInterfacePtr & _soundVolumeProvider ) override;
                
    public:
		void onTurnStream( bool _turn ) override;
        void onTurnSound( bool _turn ) override;

    public:
        SoundIdentityInterfacePtr createSoundIdentity( bool _isHeadMode, const SoundBufferInterfacePtr & _sample, ESoundSourceType _type, bool _streamable ) override;

    public:
		SoundBufferInterfacePtr createSoundBufferFromFile( const ConstString& _pakName, const FilePath & _filePath, const ConstString & _codecType, bool _streamable ) override;

	protected:
		SoundDecoderInterfacePtr createSoundDecoder_( const ConstString& _pakName, const FilePath & _filePath, const ConstString & _codecType, bool _streamable );

    public:
		void setSoundVolume( const ConstString & _type, float _volume, float _default ) override;
		float getSoundVolume( const ConstString & _type ) const override;
		float mixSoundVolume() const override;

		void setCommonVolume( const ConstString & _type, float _volume, float _default ) override;
		float getCommonVolume( const ConstString & _type ) const override;
		float mixCommonVolume() const override;

		void setMusicVolume( const ConstString & _type, float _volume, float _default ) override;
		float getMusicVolume( const ConstString & _type ) const override;
		float mixMusicVolume() const override;

		void setVoiceVolume( const ConstString & _type, float _volume, float _default ) override;
        float getVoiceVolume( const ConstString & _type ) const override;
		float mixVoiceVolume() const override;

	public:
		bool setSourceVolume( const SoundIdentityInterfacePtr & _identity, float _volume, float _default, bool _force ) override;
		float getSourceVolume( const SoundIdentityInterfacePtr & _identity ) const override;

	public:
		bool setSourceMixerVolume( const SoundIdentityInterfacePtr & _identity, const ConstString & _mixer, float _volume, float _default ) override;
		float getSourceMixerVolume( const SoundIdentityInterfacePtr & _identity, const ConstString & _mixer ) const override;

	public:
        bool releaseSoundSource( const SoundIdentityInterfacePtr & _identity ) override;
		
	public:
		bool playEmitter( const SoundIdentityInterfacePtr & _identity ) override;
		bool pauseEmitter( const SoundIdentityInterfacePtr & _identity ) override;
		bool resumeEmitter( const SoundIdentityInterfacePtr & _identity ) override;
		bool stopEmitter( const SoundIdentityInterfacePtr & _identity ) override;

	public:
		bool setLoop( const SoundIdentityInterfacePtr & _identity, bool _looped ) override;
		bool getLoop( const SoundIdentityInterfacePtr & _identity ) const override;

	public:
		void setSourceListener( const SoundIdentityInterfacePtr & _identity, const SoundListenerInterfacePtr & _listener ) override;
		
	public:
		float getDuration( const SoundIdentityInterfacePtr & _identity ) const override;
		
		bool setPosMs( const SoundIdentityInterfacePtr & _identity, float _pos ) override;
		float getPosMs( const SoundIdentityInterfacePtr & _identity ) override;

		void mute( bool _mute ) override;
		bool isMute() const override;

	public:
		void updateVolume() override;

    protected:
		void updateSoundVolumeProvider_( const SoundVolumeProviderInterfacePtr & _provider );
		void updateSourceVolume_( const SoundIdentityPtr & _source );
        
    protected:
        void playSounds_();
        void pauseSounds_();
        void stopSounds_();      
        
    protected:
        bool stopSoundBufferUpdate_( const SoundIdentityPtr & _source );
        bool playSoundBufferUpdate_( const SoundIdentityPtr & _source );

	protected:
        MixerValue m_commonVolume;
		MixerValue m_soundVolume;
		MixerValue m_musicVolume;
        MixerValue m_voiceVolume;

        uint32_t m_enumerator;

        FactoryPtr m_factorySoundEmitter;

		typedef Vector<SoundIdentityPtr> TVectorSoundSource;
		TVectorSoundSource m_soundIdentities;
		
        ThreadJobPtr m_threadJobSoundBufferUpdate;

        FactoryPtr m_factoryWorkerTaskSoundBufferUpdate;

		typedef stdex::vector<SoundVolumeProviderInterfacePtr> TVectorSoundVolumeProviders;
		TVectorSoundVolumeProviders m_soundVolumeProviders;

		bool m_supportStream;
		bool m_muted;

		bool m_turnStream;
		bool m_turnSound;		
	};
};
