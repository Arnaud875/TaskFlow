import React, { useState } from "react";
import { ConfigProvider, Typography, Button, Modal } from "antd";
const { Title } = Typography;

export default function App() {
  const [isModelOpen, setIsModelOpen] = useState(false);

  const showModal = () => {
    setIsModelOpen(true);
  };

  const handleOkAndCancel = () => {
    setIsModelOpen(false);
  };

  return (
    <ConfigProvider>
      <div style={{
        height: '100vh',
        display: 'flex',
        flexDirection: 'column',
        justifyContent: 'center',
        alignItems: 'center',
        gap: '20px'
      }}>
        <Title level={1}>Hello, World !</Title>
        <Button type="primary" size="large" onClick={showModal}>
          Click Me!
        </Button>
      </div>

      <Modal title="Hello, World !" open={isModelOpen} onOk={handleOkAndCancel} onCancel={handleOkAndCancel}>
        <p>Hello my friend !</p>
      </Modal>
    </ConfigProvider>
  )
}
